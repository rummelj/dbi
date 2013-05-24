/* 
 * File:   FreeSpaceInventory.hpp
 * Author: alexx
 *
 * Created on 24. Mai 2013, 12:01
 */

#ifndef FREESPACEINVENTORY_HPP
#define	FREESPACEINVENTORY_HPP

namespace dbi {

    class Segment;
    class SegmentManager;

    class FreeSpaceInventory : public Segment {
    public:

        FreeSpaceInventory(SegmentManager& segmentManager);
        virtual ~FreeSpaceInventory();

    };

}


#endif	/* FREESPACEINVENTORY_HPP */

