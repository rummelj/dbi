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

    BufferFrame::BufferFrame(void* data) : 
    _pageId(0),
    _data(data),
    _exclusive(false),
    _dirty(false),
    _fixCount(0) {
        
        pthread_mutex_init(&_exclusive_mutex, NULL);

        std::clog << "BufferFrame created for " << data << std::endl;
    }

    BufferFrame::~BufferFrame() {
        //@todo: Delete data? Cannot delete void*

        std::clog << "BufferFrame destroyed for " << _data << std::endl;
        pthread_mutex_destroy(&_exclusive_mutex);
        pthread_cond_destroy(&_exclusive_changed);
    }

    void* BufferFrame::getData() {
        return _data;
    }

}

