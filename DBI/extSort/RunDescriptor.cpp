/* 
 * File:   Run.cpp
 * Author: alexx
 * 
 * Created on 25. April 2013, 19:45
 */

#include <cstdio>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "file_descriptor.hpp"
#include "RunDescriptor.hpp"

using namespace dbi;

RunDescriptor::RunDescriptor(uint64_t number) : _number(number) {
    snprintf( _fileName, MAX_FILENAME_LENGTH, "%s%lu", FILENAME_PREFIX, number );
}

RunDescriptor::~RunDescriptor() {
}

int RunDescriptor::createAndOpen() const {
    return open(_fileName, O_WRONLY | O_CREAT, S_IROTH | S_IRGRP | S_IWUSR | S_IRUSR );
}

int RunDescriptor::openForRead() const {
    return open( _fileName, O_RDONLY );
}

void RunDescriptor::createAndWrite( const uint64_t* chunk, const size_t numElements ) const {
    
    const uint64_t numBytes = numElements * sizeof(uint64_t);
    
    file_descriptor runFd( createAndOpen() );
    
    runFd.preallocate( numBytes );
    write( runFd, chunk, numBytes );
    
}

int RunDescriptor::remove() const {
    return unlink( _fileName );
}