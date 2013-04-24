/* 
 * File:   outputBuffer.cpp
 * Author: alexx
 * 
 * Created on 24. April 2013, 13:27
 */

#include "outputBuffer.h"

outputBuffer::outputBuffer( int fd, uint64_t bufferSize )  {
}

outputBuffer::~outputBuffer() {
    delete[] _buffer;
}

