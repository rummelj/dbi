/* 
 * File:   file_descriptor.cpp
 * Author: alexx
 * 
 * Created on 25. April 2013, 09:05
 */

#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <bits/stat.h>
#include <assert.h>

#include "file_descriptor.hpp"

using namespace dbi;

file_descriptor::file_descriptor(int fd) {
    if(fd < 0) {
        std::cerr << "Unable to open file descriptor: " << fd << std::endl;
        assert(false);
    }
    
    _fd = fd;
}

file_descriptor::~file_descriptor() {
    close(_fd);
}

file_descriptor::operator const int&() {
    return _fd;
}

uint64_t file_descriptor::getSize() {
    struct stat fileInfo;
    
    fstat( _fd, &fileInfo );
    
    return fileInfo.st_size;
}