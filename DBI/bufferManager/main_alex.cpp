/* 
 * File:   main_alex.cpp
 * Author: alexx
 *
 * Created on 18. Mai 2013, 09:19
 */

#include <cstdlib>
#include <iostream>

#include <glog/logging.h>

#include "BufferManager.hpp"
#include "BufferFrame.hpp"

class NullStrategy {
    
public:
    
        NullStrategy() {
            LOG(INFO) << "Strategy created.";
        }

        virtual ~NullStrategy() {
            LOG(INFO) << "Strategy destructed.";
        }

        /**
         * The given id is under eviction management after calling this method.
         * @param id
         */
        void know(uint64_t id) {
            LOG(INFO) << "know " << id;
        }

        /**
         * The given id is not under eviction management anymore after calling this method.
         * @param id
         */
        void forget(uint64_t id) {
            LOG(INFO) << "forget " << id;
        }

        /**
         * Indicates that a given id was used.
         * @param id
         */
        void used(uint64_t id) {
            LOG(INFO) << "used " << id;
        }

        /**
         * The known id given is not a candidate for eviction anymore.
         * @param id
         */
        void preserve(uint64_t id) {
            LOG(INFO) << "preserve " << id;
        }

        /**
         * The known id again is a candidate for eviction now.
         * @param id
         */
        void release(uint64_t id) {
            LOG(INFO) << "release " << id;
        }

        /**
         * Returns the next id that should be evicted but does not forget it.
         * @return 
         * 0 if nothing is evictable.
         * 
         */
        uint64_t getEvictable() {
            LOG(INFO) << "evictable";
            return 0;
        }

        /**
         * @return 
         * number of known elements
         */
        unsigned int size() {
            return 0;
        }
    
};

using namespace dbi;

/*
 * 
 */
int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);

    BufferManager<NullStrategy> manager("lorem.db", 2);
    
    for(unsigned int i = 0; i < 3; i++) {
        BufferFrame& frame = manager.fixPage(i, false);
        std::cout << reinterpret_cast<char*>(frame.getData())[0] << std::endl;
        manager.unfixPage(frame, false);
    }
    
    return EXIT_SUCCESS;
}

