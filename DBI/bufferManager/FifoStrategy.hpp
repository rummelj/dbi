/* 
 * File:   FifoStrategy.hpp
 * Author: alexx
 *
 * Created on 18. Mai 2013, 09:53
 */

#ifndef FIFOSTRATEGY_HPP
#define	FIFOSTRATEGY_HPP

#include <cstdint>

#include <deque>
#include <unordered_map>

namespace dbi {

    class FifoStrategy {
        
        struct fifo_t {
            uint64_t page;
            bool preserved;
        };
        
    public:
        FifoStrategy();

        /**
         * The given id is under eviction management after calling this method.
         * @param id
         */
        void know(uint64_t id);

        /**
         * The given id is not under eviction management anymore after calling this method.
         * @param id
         */
        void forget(uint64_t id);

        /**
         * Indicates that a given id was used.
         * @param id
         */
        void used(uint64_t id);

        /**
         * The known id given is not a candidate for eviction anymore.
         * @param id
         */
        void preserve(uint64_t id);

        /**
         * The known id again is a candidate for eviction now.
         * @param id
         */
        void release(uint64_t id);

        /**
         * Returns the next id that should be evicted but does not forget it.
         * @return 
         * 0 if nothing is evictable.
         * 
         */
        uint64_t getEvictable();

        /**
         * @return 
         * number of known elements
         */
        unsigned int size();
        
    private:
        
        std::deque<uint64_t> _fifo;
        std::unordered_map<uint64_t, bool> _fifoMap;
        
        
    };
}


#endif	/* FIFOSTRATEGY_HPP */

