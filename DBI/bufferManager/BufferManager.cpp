/* 
 * File:   BufferManager.cpp
 * Author: johannes
 * 
 * Created on May 2, 2013, 4:32 PM
 */

#include <string>
#include <map>

#include <cstdio>
#include <unistd.h>

#include <iostream>

#include "BufferManager.hpp"

using namespace std;

namespace dbi {

    BufferManager::BufferManager(const std::string& filename, uint64_t size)
    : _pageFileManager(filename) {
        _size = size;
        pthread_mutex_init(&_new_frame, NULL);
    }

    BufferManager::~BufferManager() {
        pthread_mutex_destroy(&_new_frame);

        //Write all dirty pages
        for (map<uint64_t, BufferFrame*>::iterator it = _buffer.begin(); it != _buffer.end(); ++it) {
            uint64_t pageId = it->first;
            BufferFrame* bf = it->second;

            if (bf->_dirty) {
                _pageFileManager.writePage(pageId, bf->getData());
            }

            delete(bf);
        }
    }

    BufferFrame& BufferManager::fixPage(uint64_t pageId, bool exclusive) {
        if (!isInBuffer(pageId)) {
            pthread_mutex_lock(&_new_frame);
            if (!isInBuffer(pageId)) {
                BufferFrame* bf = new BufferFrame(_pageFileManager.readPage(pageId));
                saveInBuffer(pageId, *bf);
            }
            pthread_mutex_unlock(&_new_frame);
        }

        std::clog << "fixing " << pageId << " exclusive " << exclusive << std::endl;

        BufferFrame& bf = getFromBuffer(pageId);
        pthread_mutex_lock(&bf._exclusive_mutex);
        if (exclusive) {
            while (bf._fixCount > 0) {
                pthread_cond_wait(&bf._exclusive_changed, &bf._exclusive_mutex);
            }
            _twoQ.preserve(pageId);
            bf._exclusive = true;
        } else {
            while (bf._exclusive == true) {
                pthread_cond_wait(&bf._exclusive_changed, &bf._exclusive_mutex);
            }
        }
        bf._fixCount++;
        bf._pageId = pageId;
        pthread_mutex_unlock(&bf._exclusive_mutex);
        return bf;
    }

    void BufferManager::unfixPage(BufferFrame& frame, bool isDirty) {
        pthread_mutex_lock(&frame._exclusive_mutex);

        std::clog << "unfixing " << frame._pageId << " dirty " << isDirty << std::endl;

        if (isDirty && !frame._exclusive) {
            //@todo: Cannot make a frame dirty that was not held exclusively

            perror("unfixPage");
            pthread_mutex_unlock(&frame._exclusive_mutex);
            throw 42;
        }

        frame._fixCount--;
        frame._dirty = frame._dirty || isDirty;
        if (frame._fixCount == 0 || frame._exclusive) {
            std::clog << "releasing the kraken" << std::endl;
            _twoQ.release(frame._pageId);
            frame._exclusive = false;
            pthread_cond_broadcast(&frame._exclusive_changed);
        }

        pthread_mutex_unlock(&frame._exclusive_mutex);
    }

    bool BufferManager::isInBuffer(uint64_t pageId) {
        return _buffer.count(pageId) == 1;
    }

    BufferFrame& BufferManager::getFromBuffer(uint64_t pageId) {
        return *_buffer.find(pageId)->second;
    }

    void BufferManager::saveInBuffer(uint64_t pageId, BufferFrame& bufferFrame) {
        while (_buffer.size() >= _size) {
            uint64_t evictable = _twoQ.getEvictable();

            //            while ((evictable = ) == 0) {
            //                
            //                
            //                //@todo evict busy waiting
            //                usleep(1000);
            //                
            //                //perror("evictable");
            //                //@todo
            //                //throw 42;
            //            }

            clearFromBuffer(evictable);
        }
        _twoQ.know(pageId);
        _buffer[pageId] = &bufferFrame;
    }

    void BufferManager::clearFromBuffer(uint64_t pageId) {
        if (isInBuffer(pageId)) {
            //Write dirty page
            BufferFrame& bf = getFromBuffer(pageId);
            if (bf._dirty) {
                _pageFileManager.writePage(pageId, bf.getData());
            }

            //@todo: Delete bufferframe from heap?
            _twoQ.forget(pageId);
            _buffer.erase(pageId);
        }
    }
}

