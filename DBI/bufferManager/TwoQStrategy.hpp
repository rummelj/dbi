/* 
 * File:   TwoQStrategy.hpp
 * Author: johannes
 *
 * Created on May 2, 2013, 11:40 PM
 */

#ifndef TWOQSTRATEGY_HPP
#define	TWOQSTRATEGY_HPP

#include <map>
#include <string>
#include <pthread.h>

#include "EnrichedQueue.hpp"

namespace dbi {

    class TwoQStrategy {
    public:
        TwoQStrategy();

        virtual ~TwoQStrategy();

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
        EnrichedQueue<uint64_t> _fifo;
        EnrichedQueue<uint64_t> _lru;
        std::map<uint64_t, bool> _preserved;

        pthread_mutex_t _fifo_mutex;
        pthread_mutex_t _lru_mutex;
        pthread_mutex_t _preserved_mutex;
        
        pthread_cond_t _preserved_changed = PTHREAD_COND_INITIALIZER;

        void lockFifo();
        void unlockFifo();
        
        void lockLru();
        void unlockLru();
        
        void setPreserved(uint64_t id, bool value);
        void lockPreserved();
        void unlockPreserved();
    };
}
#endif	/* TWOQSTRATEGY_HPP */

