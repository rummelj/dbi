/* 
 * File:   BufferManager.cpp
 * Author: johannes
 * 
 * Created on May 2, 2013, 4:32 PM
 */

#include <string>
#include <map>

#include "BufferManager.hpp"
#include "BufferFrame.hpp"
#include "PageFileManager.hpp"

using namespace std;

namespace dbi {
    namespace bm {

        BufferManager::BufferManager(const std::string& filename, uint64_t size)
        : _pageFileManager(filename) {
            _size = size;
        }

        BufferManager::~BufferManager() {
            
        }

        BufferFrame& BufferManager::fixPage(uint64_t pageId, bool exclusive) {

        }

        void BufferManager::unfixPage(BufferFrame& frame, bool isDirty) {

        }

        bool BufferManager::isInBuffer(uint64_t pageId){
            return _buffer.count(pageId) == 1;
        }
        
        BufferFrame& BufferManager::getFromBuffer(uint64_t pageId){
            return *_buffer.find(pageId)->second;
        }
        
        void BufferManager::saveInBuffer(uint64_t pageId, BufferFrame& bufferFrame){
            _buffer[pageId] = &bufferFrame;
        }
    }
}

