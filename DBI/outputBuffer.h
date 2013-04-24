/* 
 * File:   outputBuffer.h
 * Author: alexx
 *
 * Created on 24. April 2013, 13:27
 */

#ifndef OUTPUTBUFFER_H
#define	OUTPUTBUFFER_H

#include <cstdint>
#include <cstdlib>

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

    int _fd;
    
    uint64_t* _buffer;
    uint64_t _pos;
    
    uint64_t bufferSize;
    
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
    size_t write( uint64_t* buffer, uint64_t len, uint64_t offset = 0 );

    /**
     * Writes a single element to the output buffer
     * 
     * @param element
     * @return 
     */
    size_t write( uint64_t element );
    
    /**
     * Writes the remaining elements in the buffer (from 0 to cur) to the output file
     */
    void flush();
};

#endif	/* OUTPUTBUFFER_H */

