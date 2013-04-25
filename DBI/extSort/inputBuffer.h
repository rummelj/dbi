/* 
 * File:   inputBuffer.h
 * Author: alexx
 *
 * Created on 24. April 2013, 13:06
 */

#ifndef INPUTBUFFER_H
#define	INPUTBUFFER_H

#include <cstdint>
#include <stdio.h>

class inputBuffer {
public:
    inputBuffer( int fd, uint64_t bufferSize );
    virtual ~inputBuffer();

private:

    const size_t DATA_SIZE = sizeof (uint64_t);

    int _fd; // fd = file descriptor, quasi ein zeiger auf eine datei
    uint64_t _bufferSize; // größe des buffers ( _buffer wird dann _bufferSize Ts enthalten können)

    uint64_t* _buffer;

    size_t _cur;

    /**
     * reads (max) bufferSize 'T's into the buffer and returns the actual number of bytes read
     * 
     * @return the number of bytes read
     */
    size_t loadNextChunk();

public:


    /**
     * Reads the next element from the file into dest
     * This loads a new chunk when necessary
     * 
     * @param dest the destination where to write the element
     * 
     * @return the number of bytes read or -1 if something went wrong
     */
    size_t getNextElement(uint64_t& dest);

    /**
     * Reads the next chunk and passes it to the pointer in dest
     * 
     * @param dest the destination pointer
     * 
     * @return the number of bytes read or -1 if something went wrong
     */
    size_t getNextChunk(uint64_t*& dest);

    /**
     * @return whether the stream is at its end
     */
    bool isEOF();

    /**
     * Returns the filesize of the managed file
     * @return the filesize in bytes
     */
    off64_t getFileSize();

};

#endif	/* INPUTBUFFER_H */

