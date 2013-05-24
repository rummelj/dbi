/* 
 * File:   SegmentManager.hpp
 * Author: alexx
 *
 * Created on 20. Mai 2013, 11:27
 */

#ifndef SEGMENTMANAGER_HPP
#define	SEGMENTMANAGER_HPP

#include <cstdint>

#include "SegmentInventory.hpp"
#include "FreeSpaceInventory.hpp"

namespace dbi {

    class FifoStrategy;
    class PageFileManager;
    class BufferManager;
    class BufferFrame;
    class SegmentInventory;

    class SegmentManager {
        static const uint64_t SEGMENT_INVENTORY_PAGEID = 0;
        
        


    public:
        SegmentManager(BufferManager& bufferManager);
        virtual ~SegmentManager();


        /**
         * Initializes the segmentmanager for it's first use.
         * That is:
         *  - initializing the segment inventory
         *  - initializing the free space inventory
         */
        void initForFirstUse();
        
        BufferFrame& fixPage( uint64_t pageId, bool exclusive );
        void unfixPage( BufferFrame& frame, bool isDirty );
        
        PageRange allocate(uint64_t min, uint64_t max);

    private:

        BufferManager& _bufferManager;
        
        SegmentInventory* _segmentInventory;
        FreeSpaceInventory* _freeSpaceInventory;



    };

}

#endif	/* SEGMENTMANAGER_HPP */

