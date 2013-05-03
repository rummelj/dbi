/* 
 * File:   BufferManager.cpp
 * Author: johannes
 * 
 * Created on May 2, 2013, 4:32 PM
 */

#include <string>
#include <map>

#include "BufferManager.hpp"

using namespace std;

namespace dbi {

    BufferManager::BufferManager(const std::string& filename, uint64_t size)
    : _pageFileManager(filename) {
        _size = size;
    }

    BufferManager::~BufferManager() {
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
        if (isInBuffer(pageId)) {
            BufferFrame& bf = getFromBuffer(pageId);
            pthread_mutex_lock(&bf._exclusive_mutex);
            if (exclusive) {
                if (bf._fixCount > 0) {
                    pthread_cond_wait(&bf._exclusive_changed, &bf._exclusive_mutex);
                }
                _twoQ.preserve(pageId);
                bf._exclusive = true;
            } else {
                if (bf._exclusive == true) {
                    pthread_cond_wait(&bf._exclusive_changed, &bf._exclusive_mutex);
                }
            }
            pthread_mutex_unlock(&bf._exclusive_mutex);
            bf._fixCount++;
            bf._pageId = pageId;
            return bf;
        } else {
            BufferFrame* bf = new BufferFrame(_pageFileManager.readPage(pageId));
            saveInBuffer(pageId, *bf);
            return fixPage(pageId, exclusive);
        }
    }

    void BufferManager::unfixPage(BufferFrame& frame, bool isDirty) {
        if (frame._dirty && !frame._exclusive) {
            //@todo: Cannot make a frame dirty that was not held exclusively
            throw 42;
        }

        frame._fixCount--;
        frame._dirty = frame._dirty || isDirty;
        if (frame._fixCount == 0 || frame._exclusive) {
            _twoQ.release(frame._pageId);
            frame._exclusive = false;
            pthread_cond_signal(&frame._exclusive_changed);
        }
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
            if (evictable == 0) {
                //@todo
                throw 42;
            }
            clearFromBuffer(evictable);
        }
        _twoQ.know(pageId);
        _buffer[pageId] = &bufferFrame;
    }

    void BufferManager::clearFromBuffer(uint64_t pageId) {
        if (isInBuffer(pageId)) {
            //Write dirty page
            BufferFrame bf = getFromBuffer(pageId);
            if (bf._dirty) {
                _pageFileManager.writePage(pageId, bf.getData());
            }

            //@todo: Delete bufferframe from heap?
            _twoQ.forget(pageId);
            _buffer.erase(pageId);
        }
    }
}

