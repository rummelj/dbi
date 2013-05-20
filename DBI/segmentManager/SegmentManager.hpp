/* 
 * File:   SegmentManager.hpp
 * Author: alexx
 *
 * Created on 20. Mai 2013, 11:27
 */

#ifndef SEGMENTMANAGER_HPP
#define	SEGMENTMANAGER_HPP

#include <cstdint>

namespace dbi {

    class FifoStrategy;
    class PageFileManager;
    class BufferManager;
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

    private:

        BufferManager& _bufferManager;



    };

}

#endif	/* SEGMENTMANAGER_HPP */

