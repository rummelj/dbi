/* 
 * File:   BufferManager.hpp
 * Author: johannes
 *
 * Created on May 2, 2013, 4:32 PM
 */

#ifndef BUFFERMANAGER_HPP
#define	BUFFERMANAGER_HPP

#include <string>
#include <unordered_map>
#include <queue>

#include <cstdio>
#include <cassert>

#include <glog/logging.h>


#include "BufferFrame.hpp"
#include "PageFileManager.hpp"
#include "TwoQStrategy.hpp"

using namespace std;

namespace dbi {

    template<class RepressionStrategy, class FileManager = PageFileManager>
    class BufferManager {
        
    public:
        /**
         * Create a new instance that manages size frames and operates 
         * on the file filename
         * @param filename
         * @param size
         */
        BufferManager(const std::string& filename, uint64_t size);

        /**
         * A method to retrieve frames given a page ID and indicating 
         * whether the page will be held exclusively by this thread or not.
         * The method can fail if no free frame is available and no used 
         * frame can be freed.
         * @param pageId
         * @param exclusive
         * @return 
         */
        BufferFrame& fixPage(uint64_t pageId, bool exclusive);

        /**
         * Return a frame to the buffer manager indicating whether it is 
         * dirty or not. If dirty, the page manager must write it back to 
         * disk. It does not have to write it back immediately, but must 
         * not write it back before unfixPage is called.
         * @param frame
         * @param isDirty
         */
        void unfixPage(BufferFrame& frame, bool isDirty);

        /**
         * Write all dirty frames to disk and free all resources.
         */
        virtual ~BufferManager();
        
        void checkDictionary();

    protected:
        uint64_t _size;
        FileManager _fileManager;



    private:
        unordered_map<uint64_t, BufferFrame*> _dictionary;
        
        RepressionStrategy _repressionStrategy;
        
        pthread_mutex_t _new_frame;
        
        bool isInDictionary(uint64_t pageId);
        BufferFrame& getFromDictionary(uint64_t pageId);
        void saveInDictionary(BufferFrame& bufferFrame);
        void clearFromDictionary(uint64_t pageId);
    };

    
    //=============IMPLEMENTATION================
    
    
    template<class RepressionStrategy, class FileManager>
    BufferManager<RepressionStrategy, FileManager>::BufferManager(const std::string& filename, uint64_t size)
    : _fileManager(filename) {
        _size = size;

    }

    template<class RepressionStrategy, class FileManager>
    BufferManager<RepressionStrategy, FileManager>::~BufferManager() {
        
        LOG(INFO) << "Destructing BufferManager.";
        
        for (unordered_map<uint64_t, BufferFrame*>::iterator it = _dictionary.begin(); it != _dictionary.end(); ++it) {
            uint64_t pageId = it->first;
            BufferFrame* bf = it->second;

            if (bf->isDirty()) {
                _fileManager.writePage(pageId, bf->getData());
            } 

            _fileManager.closePage(pageId, bf->getData());

            delete bf;
        }
        
    }
    
    template<class RepressionStrategy, class FileManager>
    void BufferManager<RepressionStrategy, FileManager>::checkDictionary() {
        
        LOG(INFO) << "vvvvvvvv" << " Check Dictionary";
        
        for (unordered_map<uint64_t, BufferFrame*>::iterator it = _dictionary.begin(); it != _dictionary.end(); ++it) {
            uint64_t pageId = it->first;
            BufferFrame* bf = it->second;

            if(pageId != bf->getPageId()) {
                LOG(ERROR) << "Frame #" << bf->getPageId() << " saved at wrong key #" << pageId;
            } else {
                LOG(INFO) << "Everything okay with Frame #" << pageId;
            }
        }
        
        LOG(INFO) << "^^^^^^^^" << " End Check Dictionary";
        
    }

    template<class RepressionStrategy, class FileManager>
    BufferFrame& BufferManager<RepressionStrategy, FileManager>::fixPage(uint64_t pageId, bool exclusive) {
        
        LOG(INFO) << "Fix Page #" << pageId;
        
        //lock dictionary
        
        if( !isInDictionary( pageId ) ) {
            
            LOG(INFO) << "Page #" << pageId << " not in buffer.";
            
            void* pageData = _fileManager.readPage(pageId);
            
            BufferFrame* newFrame = new BufferFrame(pageId, pageData);
            saveInDictionary( *newFrame );
        }
        
        LOG(INFO) << "Fixing page #" << pageId << ", exclusive: " << (exclusive ? "t" : "f");
        
        
        BufferFrame& frame = getFromDictionary(pageId);
        
        if(exclusive) {
            frame.lock( BufferFrame::LockIntend::Intend_Exclusive );
        } else {
            frame.lock( BufferFrame::LockIntend::Intend_Shared );
        }
        
        _repressionStrategy.preserve(pageId);
        
        return frame;
        
        //unlock dictionary
        
    }

    template<class RepressionStrategy, class FileManager>
    void BufferManager<RepressionStrategy, FileManager>::unfixPage(BufferFrame& frame, bool isDirty) {
        
        LOG(INFO) << "Unfix Page #" << frame.getPageId();
        
        if(!isInDictionary(frame.getPageId())) {
            LOG(FATAL) << "BufferFrame to be unfixed not in dictionary anymore.";
            assert(false);
        }
        
        uint64_t numLocks = frame.removeLock(isDirty);
 
        if(numLocks == 0) {
            _repressionStrategy.release(frame.getPageId());
        }
        
    }

    template<class RepressionStrategy, class FileManager>
    bool BufferManager<RepressionStrategy, FileManager>::isInDictionary(uint64_t pageId) {
        return _dictionary.find(pageId) != _dictionary.end();
    }

    template<class RepressionStrategy, class FileManager>
    BufferFrame& BufferManager<RepressionStrategy, FileManager>::getFromDictionary(uint64_t pageId) {
        
        if(!isInDictionary(pageId)) {
            LOG(FATAL) << "Page #" << pageId << " not in dict.";
            assert(false);
        }
        
        if( _dictionary[pageId]->getPageId() != pageId  ) {
            LOG(FATAL) << "Frame in dict at key #" << pageId << " has Id #" << _dictionary[pageId]->getPageId() <<" Data: " << _dictionary[pageId]->getData();
            assert(false);
        }
        
        BufferFrame& frame = *_dictionary[pageId];
                
        return frame;
    }

    template<class RepressionStrategy, class FileManager>
    void BufferManager<RepressionStrategy, FileManager>::saveInDictionary(BufferFrame& bufferFrame) {
        
        LOG(INFO) << "Saving Page #" << bufferFrame.getPageId() << " Data: " << bufferFrame.getData();
        
        if( _dictionary.size() >= _size ) {
            LOG(INFO) << "Dictionary full";
            
            uint64_t evictablePage = _repressionStrategy.getEvictable();
            clearFromDictionary(evictablePage);
        }
        
        
        _dictionary[bufferFrame.getPageId()] = &bufferFrame;

        _repressionStrategy.know(bufferFrame.getPageId());
    }

    template<class RepressionStrategy, class FileManager>
    void BufferManager<RepressionStrategy, FileManager>::clearFromDictionary(uint64_t pageId) {
        
        LOG(INFO) << "Evicting page #" << pageId;
        
        if(!isInDictionary(pageId)) {
            LOG(FATAL) << "Page #" << pageId << " not in dictionary.";
            assert(false);
        }
        
        BufferFrame& frame = getFromDictionary(pageId);
        
        if(frame.isLocked()) {
            LOG(FATAL) << "Page #" << pageId << " is still locked.";
            assert(false);
        }
        
        if(frame.isDirty()) {
                _fileManager.writePage(pageId, frame.getData());
        }
        
        _fileManager.closePage(pageId, frame.getData());
        
        _repressionStrategy.forget(pageId);
        _dictionary.erase(pageId);
        
        
        delete &frame;
    }
    
}
#endif	/* BUFFERMANAGER_HPP */

