/* 
 * File:   BufferFrame.cpp
 * Author: johannes
 * 
 * Created on May 2, 2013, 4:42 PM
 */

#include <stdlib.h>
#include <pthread.h>

#include "BufferFrame.hpp"

namespace dbi {

    BufferFrame::BufferFrame(void* data) {
        _data = data;
        pthread_mutex_init(&_exclusive_mutex, NULL);
    }

    BufferFrame::~BufferFrame() {
        //@todo: Delete data? Cannot delete void*
        pthread_mutex_destroy(&_exclusive_mutex);
    }

    void* BufferFrame::getData() {
        return _data;
    }

}

