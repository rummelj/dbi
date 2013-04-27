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

#include "RunDescriptor.hpp"

using namespace dbi;

RunDescriptor::RunDescriptor(uint64_t number) : _number(number) {
    snprintf( _fileName, MAX_FILENAME_LENGTH, "%s%lld", FILENAME_PREFIX, number );
}

RunDescriptor::~RunDescriptor() {
}

int RunDescriptor::createAndOpen() {
    return open(_fileName, O_WRONLY | O_CREAT, S_IROTH | S_IRGRP | S_IWUSR | S_IRUSR );
}

int RunDescriptor::openForRead() {
    return open( _fileName, O_RDONLY );
}

int RunDescriptor::remove() {
    return unlink( _fileName );
}