
#include <string.h>

#include "Segment.hpp"
#include "SegmentInventory.hpp"

#include "../MemoryDataStream.hpp"

namespace dbi {

    SegmentInventory::SegmentInventory(SegmentManager& segmentManager)
    : Segment(segmentManager, 0, ProtectionType::PROT_PRIVATE, PersistencyType::PERS_PREMANENT) {
        
        
        
    }

    SegmentInventory::~SegmentInventory() {

        for( auto& ranges : _segmentRanges ) {
            for( auto& range : ranges ) {
                delete range;
            }
        }
        
        for( auto& segment : _segmentInfos ) {
            delete segment;
        }
        
    }
    
    
    void SegmentInventory::init() {
        _header.numSegments = 0;
        
        save();
    }
    
    void SegmentInventory::load() {
        
        BufferFrame& frame = _segmentManager.fixPage(SEGMENT_INVENTORY_PAGEID, false);
        
        MemoryDataStream mds(frame.getData(), 4096); //4096 == pagesize; optimize out
        
        mds.copy( &_header, sizeof(SegmentInventoryHeader) );
        
        for( unsigned int i = 0; i < _header.numSegments; i++ ) {
            SegmentInfo* info = new SegmentInfo;
            mds.copy( info, sizeof(SegmentInfo) );
            
            _segmentInfos.push_back(info);
            
            for( unsigned int j = 0; j < info.rangeCount; j++ ) {
                PageRange* nextRange = new PageRange;
                
                mds.copy( nextRange, sizeof(PageRange) );
                
                
                _segmentRanges[info->segmentId].push_back(nextRange);
            }
        }
        
        _segmentManager.unfixPage(frame, false);
    }
    
    void SegmentInventory::save() {
        
        BufferFrame& frame = _segmentManager.fixPage(SEGMENT_INVENTORY_PAGEID, true);
        
        MemoryDataStream mds(frame.getData(), 4096); //4096 == pagesize; optimize out
        
        _header.numSegments = _segmentInfos.size();
        
        mds.write(&_header, sizeof(SegmentInventoryHeader));
        
        for( unsigned int i = 0; i < _header.numSegments; i++ ) {
            
            SegmentInfo* si = _segmentInfos[i];
            std::vector<PageRange*>& ranges = _segmentRanges[si->segmentId];
            
            si->rangeCount = ranges.size();
            
            mds.write(si, sizeof(SegmentInfo));
            
            
            for( unsigned int j = 0; j < si->rangeCount; j++ ) {
                PageRange* pr = ranges[j];
                
                mds.write(pr, sizeof(PageRange));
            }
            
        }
        
        
        _segmentManager.unfixPage(frame, true);
        
    }

}