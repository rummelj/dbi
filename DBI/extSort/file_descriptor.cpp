/* 
 * File:   file_descriptor.cpp
 * Author: alexx
 * 
 * Created on 25. April 2013, 09:05
 */

#include <cstdio>
#include <cassert>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <bits/stat.h>
#include <fcntl.h>

#include "file_descriptor.hpp"

using namespace dbi;

file_descriptor::file_descriptor(int fd) {
    if(fd < 0) {
        perror("fd");
        assert(false);
    }
    
    _fd = fd;
}

file_descriptor::~file_descriptor() {
    if(close(_fd)) {
        perror("close");
        assert(false);
    }
}

file_descriptor::operator const int&() {
    return _fd;
}

uint64_t file_descriptor::getSize() {
    struct stat fileInfo;
    
    if( fstat( _fd, &fileInfo ) < 0) {
        perror("fstat");
        assert(false);
    }
    
    return fileInfo.st_size;
}

void file_descriptor::preallocate( uint64_t len, uint64_t offset ) {
    
    if( posix_fallocate( _fd, offset, len ) != 0 ) {
        perror("posix_fallocate");
        assert(false);
    }
    
}