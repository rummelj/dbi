/* 
 * File:   BufferFrame.cpp
 * Author: johannes
 * 
 * Created on May 2, 2013, 4:42 PM
 */

#include <stdlib.h>
#include <pthread.h>

#include <iostream>

#include "BufferFrame.hpp"

namespace dbi {

    BufferFrame::BufferFrame(void* data) {
        _data = data;
        pthread_mutex_init(&_exclusive_mutex, NULL);
        
        std::cerr << "BufferFrame created for " << data << std::endl;
    }

    BufferFrame::~BufferFrame() {
        //@todo: Delete data? Cannot delete void*
        
        std::cerr << "BufferFrame destroyed for " << _data << std::endl;
        pthread_mutex_destroy(&_exclusive_mutex);
    }

    void* BufferFrame::getData() {
        return _data;
    }

}

