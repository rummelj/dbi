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
#include "FifoStrategy.hpp"

#include <pthread.h>


using namespace dbi;

void* reader_thread(void* pars) {
    
    static const uint64_t multiplyer = 256;
    
    BufferManager<FifoStrategy>* bm = reinterpret_cast<BufferManager<FifoStrategy>*>(pars);
    
    for(unsigned int i = 0; i < 256 * multiplyer; i++) {
        unsigned int pageId = i % 256;
        
        BufferFrame& frame = bm->fixPage(pageId, false);
        std::cout << reinterpret_cast<char*>(frame.getData())[0] << std::endl;
        bm->unfixPage(frame, false);
    }
    
    
    return NULL;
}

void* writer_thread(void* pars) {
    
    static const uint64_t multiplyer = 256;
    
    BufferManager<FifoStrategy>* bm = reinterpret_cast<BufferManager<FifoStrategy>*>(pars);
    
    for(unsigned int i = 0; i < 256 * multiplyer; i++) {
        unsigned int pageId = i % 256;
        
        BufferFrame& frame = bm->fixPage(pageId, true);
        reinterpret_cast<char*>(frame.getData())[0] = (char)((pageId%10) + 0x30);
        bm->unfixPage(frame, true);
    }
    
    
    return NULL;
}

/*
 * 
 */
int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);

    BufferManager<FifoStrategy> manager("lorem.db", 32);

    
    pthread_t read_pthread;
    pthread_create( &read_pthread, NULL, &reader_thread, &manager );

    pthread_t write_pthread;
    pthread_create( &write_pthread, NULL, &writer_thread, &manager );
    
    pthread_join(read_pthread, NULL);
    pthread_join(write_pthread, NULL);
    
    return EXIT_SUCCESS;
}

