/* 
 * File:   Segment.hpp
 * Author: alexx
 *
 * Created on 20. Mai 2013, 11:36
 */

#ifndef SEGMENT_HPP
#define	SEGMENT_HPP

#include <cstdint>

#include <vector>

#include "SegmentManager.hpp"

namespace dbi {

    class BufferFrame;

    enum ProtectionType {
        PROT_PRIVATE,
        PROT_PUBLIC
    };

    enum PersistencyType {
        PERS_PREMANENT,
        PERS_TEMPORARY
    };

    struct PageRange {
        uint64_t offset;
        uint64_t length;
    };

    struct SegmentInfo {
        uint64_t segmentId;
        uint64_t rangeCount;

        ProtectionType protectionType;
        PersistencyType persistencyType;
    };

    class Segment {

    private:
        Segment(const Segment& orig);

    public:
        Segment(SegmentManager& segmentManager, uint64_t segmentId, ProtectionType protectionType = ProtectionType::PROT_PUBLIC, PersistencyType persistencyType = PersistencyType::PERS_PREMANENT);
        virtual ~Segment();
        
    protected:

        SegmentManager& _segmentManager;

    private:
        
        SegmentInfo _info;
        std::vector<PageRange> _pageRanges;

    public:

        void requestPages(uint64_t min, uint64_t max);

        /*
                void operator<<( const BufferFrame& sourceFrame );
                void operator>>( const BufferFrame& destinationFrame );
         */

    };




}

#endif	/* SEGMENT_HPP */

