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
#include <cstring>

namespace dbi {

template<typename T>
class OutputBuffer {
public:
    
    /**
     * creates an outputBuffer for the given filedescriptor with the given bufferSize
     * 
     * @param fd
     * @param bufferSize
     */
    OutputBuffer( int fd, uint64_t bufferSize );
    virtual ~OutputBuffer();
private:

    const size_t DATA_SIZE = sizeof (T);
    
    int _fd;
    
    char* _buffer;
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
    void writeChunk( T* buffer, uint64_t len, uint64_t offset = 0 );

    /**
     * Writes a single element to the output buffer
     * 
     * @param element
     * @return 
     */
    void writeElement( T element );
    
    /**
     * Writes the remaining elements in the buffer (from 0 to cur) to the output file
     */
    void flush();
    
    int getFd();
    
};

template<typename T>
OutputBuffer<T>::OutputBuffer( int fd, uint64_t bufferSize ) : 
        _fd(fd),
        _pos(0),
        _bufferSize(bufferSize)
         {
    
    _buffer = new char[bufferSize];
    
}

template<typename T>
OutputBuffer<T>::~OutputBuffer() {
    flush();
    delete[] _buffer;
}

template<typename T>
size_t OutputBuffer<T>::writeBackChunk() {
    if(_pos == 0) {
        return 0;
    }
    
    size_t wroteBytes = write(  _fd, _buffer, _pos  );
    
    _pos = 0;
    
    return wroteBytes;
}

template<typename T>
void OutputBuffer<T>::writeChunk( T* buffer, uint64_t len, uint64_t offset ) {
    for( uint64_t i = 0; i < len; i++ ) {
        writeElement( buffer[offset + i] );
    }
}

template<typename T>
void OutputBuffer<T>::writeElement( T element ) {
    //std::cout << element << "," << DATA_SIZE << "," << _pos << std::endl;
    memcpy( _buffer + _pos, &element, DATA_SIZE);
    
    _pos += DATA_SIZE;
    
    if(_pos >= _bufferSize) {
        writeBackChunk();
    }
}
    
template<typename T>
void OutputBuffer<T>::flush() {
    writeBackChunk();
}

template<typename T>
int OutputBuffer<T>::getFd() {
    return _fd;
}

}

#endif	/* OUTPUTBUFFER_HPP */

