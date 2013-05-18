/* 
 * File:   FifoStrategy.cpp
 * Author: alexx
 *
 * Created on 18. Mai 2013, 09:53
 */

#include "FifoStrategy.hpp"

#include <glog/logging.h>
#include <cassert>

#include <algorithm>


namespace dbi {

        FifoStrategy::FifoStrategy() {}

        /**
         * The given id is under eviction management after calling this method.
         * @param id
         */
        void FifoStrategy::know(uint64_t id) {
            LOG(INFO) << "know page #" << id;
            
            if(_fifoMap.find(id) == _fifoMap.end()) {
                _fifo.push_back( id );
                _fifoMap[id] = false;
            }
            
        }

        /**
         * The given id is not under eviction management anymore after calling this method.
         * @param id
         */
        void FifoStrategy::forget(uint64_t id) {
            LOG(INFO) << "forget page #" << id;
        
            if(_fifoMap.find(id) == _fifoMap.end()) {
                LOG(FATAL) << "Trying to forget something(Page #" << id << ") unknown.";
                assert(false);
            }
            
            _fifo.erase( std::find(_fifo.begin(), _fifo.end(), id) );
            _fifoMap.erase(id);
            
        }

        /**
         * Indicates that a given id was used.
         * @param id
         */
        void FifoStrategy::used(uint64_t id) {
            LOG(INFO) << "Page #" << id << " was used.";
        }

        /**
         * The known id given is not a candidate for eviction anymore.
         * @param id
         */
        void FifoStrategy::preserve(uint64_t id) {
            LOG(INFO) << "preserve page #" << id;
            
            if(_fifoMap.find(id) == _fifoMap.end()) {
                LOG(FATAL) << "Trying to preserve something(Page #" << id << ") unknown.";
                assert(false);
            }
            
            _fifoMap[id] = true;

        }

        /**
         * The known id again is a candidate for eviction now.
         * @param id
         */
        void FifoStrategy::release(uint64_t id) {
            LOG(INFO) << "release page #" << id;
        
            if(_fifoMap.find(id) == _fifoMap.end()) {
                LOG(FATAL) << "Trying to release something(Page #" << id << ") unknown.";
                assert(false);
            }
            
            _fifoMap[id] = false;
        }

        /**
         * Returns the next id that should be evicted but does not forget it.
         * @return 
         * 0 if nothing is evictable.
         * 
         */
        uint64_t FifoStrategy::getEvictable() {
            LOG(INFO) << "getEvictable";
            
            for( auto& id : _fifo ) {
                if(_fifoMap[id] == false) {
                    return id;
                }
            }
            
            LOG(FATAL) << "No evictable page existing.";
            assert(false);
        }

        /**
         * @return 
         * number of known elements
         */
        unsigned int FifoStrategy::size() {
            return _fifo.size();
        }
        

}


