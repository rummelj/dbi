/* 
 * File:   inputBuffer.cpp
 * Author: alexx
 * 
 * Created on 24. April 2013, 13:06
 */

#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include <cstdio>

#include "inputBuffer.h"

inputBuffer::inputBuffer( int fd, uint64_t bufferSize ) : _fd(fd), _bufferSize(bufferSize), _cur(-1) {
    _buffer = new uint64_t[bufferSize];
}

inputBuffer::~inputBuffer() {
    delete[] _buffer;
}

size_t inputBuffer::loadNextChunk() {
    _cur = 0;
    return read( _fd, _buffer, _bufferSize * DATA_SIZE);
}

size_t inputBuffer::getNextElement(uint64_t& dest) {
    //load new chunk either when no chunk was loaded before (_cur==-1), or current chunk is at the end (_cur== _bufferSize))
    // return 1 when everything was okay
}

size_t inputBuffer::getNextChunk(uint64_t*& dest) {
    size_t readBytes = loadNextChunk();
    
    dest = _buffer;
    
    return readBytes / DATA_SIZE;
}

bool inputBuffer::isEOF() {

}

off64_t inputBuffer::getFileSize() {
    
}