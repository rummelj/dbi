/* 
 * File:   Segment.hpp
 * Author: alexx
 *
 * Created on 20. Mai 2013, 11:36
 */

#ifndef SEGMENT_HPP
#define	SEGMENT_HPP

#include <cstdint>

namespace dbi {
    
    class BufferFrame;

    struct PageRange {
        uint64_t minPage;
        uint64_t maxPage;
        
        PageRange(uint64_t _minPage, uint64_t _maxPage) : minPage(_minPage), maxPage(_maxPage) {}
    };

    class Segment {
    public:

        enum ProtectionType {
            PROT_PRIVATE,
            PROT_PUBLIC
        };
        
        enum PersistencyType {
            PERS_PREMANENT,
            PERS_TEMPORARY 
       };


    private:
        Segment(const Segment& orig);

    public:
        Segment(uint64_t segmentId, uint64_t minPage, uint64_t maxPage, ProtectionType protectionType = ProtectionType::PROT_PUBLIC, PersistencyType persistencyType = PersistencyType::PERS_PREMANENT);
        virtual ~Segment();
    private:
        
        uint64_t _segmentId;
        PageRange _pageRange;
        ProtectionType _protectionType;
        PersistencyType _persistencyType;

    public:
        
        virtual void serialize( const BufferFrame& destinationFrame );
        virtual void deserialize( const BufferFrame& sourceFrame );
        
        /*
                void operator<<( const BufferFrame& sourceFrame );
                void operator>>( const BufferFrame& destinationFrame );
        */
        
    };
    
    class SegmentInventory : public Segment {
        
        public:
            
        SegmentInventory();
        virtual ~SegmentInventory();
        
    };

}

#endif	/* SEGMENT_HPP */

