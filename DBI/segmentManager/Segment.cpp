/* 
 * File:   Segment.cpp
 * Author: alexx
 * 
 * Created on 20. Mai 2013, 11:36
 */

#include "Segment.hpp"

namespace dbi {

    Segment::Segment(SegmentManager& segmentManager, uint64_t segmentId, ProtectionType protectionType, PersistencyType persistencyType)
    : _segmentManager(segmentManager)
    , _info( segmentId, 0, protectionType, persistencyType ) {

    }

    Segment::~Segment() {
    }



}

