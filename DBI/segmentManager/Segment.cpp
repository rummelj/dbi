/* 
 * File:   Segment.cpp
 * Author: alexx
 * 
 * Created on 20. Mai 2013, 11:36
 */

#include "Segment.hpp"

namespace dbi {

    Segment::Segment(uint64_t segmentId, uint64_t minPage, uint64_t maxPage, ProtectionType protectionType, PersistencyType persistencyType)
    : _segmentId(segmentId)
    , _pageRange(minPage, maxPage)
    , _protectionType(protectionType)
    , _persistencyType(persistencyType) {

    }

    Segment::~Segment() {
    }

    SegmentInventory::SegmentInventory() 
    : Segment(0, 0, 0,  ProtectionType::PROT_PRIVATE, PersistencyType::PERS_PREMANENT ) {



    }
    
    SegmentInventory::~SegmentInventory() {
        
    }

}

