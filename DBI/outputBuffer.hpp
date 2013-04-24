/* 
 * File:   outputBuffer.h
 * Author: alexx
 *
 * Created on 24. April 2013, 13:27
 */

#ifndef OUTPUTBUFFER_HPP
#define	OUTPUTBUFFER_HPP


#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include <cstdio>
#include <cstdint>
#include <cstdlib>
//#include <cstring>

template<typename T>
class outputBuffer {
public:
    
    /**
     * creates an outputBuffer for the given filedescriptor with the given bufferSize
     * 
     * @param fd
     * @param bufferSize
     */
    outputBuffer( int fd, uint64_t bufferSize );
    virtual ~outputBuffer();
private:

    const size_t DATA_SIZE = sizeof (T);
    
    int _fd;
    
    T* _buffer;
    uint64_t _pos;
    
    uint64_t _bufferSize;
    
    /**
     * Writes a chunk to the output file (from position 0 to _pos)
     * 
     * @return the number of bytes written
     */
    size_t writeBackChunk();
    
public:

    /**
     * Writes len Ts from buffer to the output buffer, beginning at offset.
     * When the buffer gets full, write the buffer into the file.
     * 
     * @param buffer the buffer which holds the data
     * @param len the number of Ts to write
     * @param offset the offset in the buffer (defaults to 0)
     * @return the number of bytes written
     */
    size_t writeChunk( T* buffer, uint64_t len, uint64_t offset = 0 );

    /**
     * Writes a single element to the output buffer
     * 
     * @param element
     * @return 
     */
    size_t writeElement( T element );
    
    /**
     * Writes the remaining elements in the buffer (from 0 to cur) to the output file
     */
    void flush();
    
};

template<typename T>
outputBuffer<T>::outputBuffer( int fd, uint64_t bufferSize ) : 
        _fd(fd),
        _bufferSize(bufferSize),
        _pos(0) {
    
    _buffer = new T[bufferSize];
    
}

template<typename T>
outputBuffer<T>::~outputBuffer() {
    flush();
    delete[] _buffer;
}

template<typename T>
size_t outputBuffer<T>::writeBackChunk() {
    if(_pos == 0) {
        return 0;
    }
    
    size_t wroteBytes = write(  _fd, _buffer, _pos * DATA_SIZE );
    
    _pos = 0;
    
    return wroteBytes;
}

template<typename T>
size_t outputBuffer<T>::writeChunk( T* buffer, uint64_t len, uint64_t offset ) {
    for( uint64_t i = 0; i < len; i++ ) {
        writeElement( buffer[offset + i] );
    }
}

template<typename T>
size_t outputBuffer<T>::writeElement( T element ) {
    _buffer[_pos] = element;
    
    _pos++;
    
    if(_pos == _bufferSize) {
        writeBackChunk();
    }
}
    
template<typename T>
void outputBuffer<T>::flush() {
    writeBackChunk();
}


#endif	/* OUTPUTBUFFER_HPP */

