/* 
 * File:   SegmentManager.cpp
 * Author: alexx
 * 
 * Created on 20. Mai 2013, 11:27
 */

#include "SegmentManager.hpp"

namespace dbi {

    SegmentManager::SegmentManager(BufferManager& bufferManager) : _bufferManager(bufferManager) {
        
        _segmentInventory = new SegmentInventory(*this);
        _segmentInventory->load();
        
    }

    SegmentManager::~SegmentManager() {
        
        delete _segmentInventory;
        
    }
    
    

    SegmentManager::initForFirstUse() {
        
        
        
        
        
    }
    
    BufferFrame& SegmentManager::fixPage( uint64_t pageId, bool exclusive ) {
        return _bufferManager.fixPage(pageId, exclusive);
    }
    
    void SegmentManager::unfixPage( BufferFrame& frame, bool isDirty ) {
        return _bufferManager.unfixPage(frame, isDirty );
    }
    
}
