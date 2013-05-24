/* 
 * File:   SegmentInventory.hpp
 * Author: alexx
 *
 * Created on 24. Mai 2013, 12:00
 */

#ifndef SEGMENTINVENTORY_HPP
#define	SEGMENTINVENTORY_HPP

#include <vector>
#include <unordered_map>

#include "Segment.hpp"


namespace dbi {

    class Segment;
    class SegmentManager;

    class SegmentInventory : public Segment {
        
        struct SegmentInventoryHeader {
            
            uint64_t numSegments;
            
        };
        
    private:
        
        static const uint64_t SEGMENT_INVENTORY_PAGEID = 0;
        
    public:

        SegmentInventory(SegmentManager& segmentManager);
        virtual ~SegmentInventory();
        
        /**
         * Initialize the SegmentInventory with size 0
         */
        void init();
        
        /**
         * Load the SegmentInventory from the database
         */
        void load();
        
        /**
         * Persist the SegmentInventory
         */
        void save();
        
        
    private:
        
        std::vector<SegmentInfo*> _segmentInfos;
        std::unordered_map<uint64_t, std::vector<PageRange*> > _segmentRanges;
        
        SegmentInventoryHeader _header;
        

    };

}


#endif	/* SEGMENTINVENTORY_HPP */

