/* 
 * File:   inputBuffer.h
 * Author: alexx
 *
 * Created on 24. April 2013, 13:06
 */

#ifndef INPUTBUFFER_HPP
#define	INPUTBUFFER_HPP

#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include <cstdio>
#include <cstdint>

template<typename T>
class inputBuffer {
public:
    inputBuffer( int fd, uint64_t bufferSize );
    virtual ~inputBuffer();

private:

    const size_t DATA_SIZE = sizeof (T);

    int _fd; // fd = file descriptor, quasi ein zeiger auf eine datei
    uint64_t _bufferSize; // größe des buffers ( _buffer wird dann _bufferSize Ts enthalten können)

    T* _buffer;

    size_t _cur;
    size_t _currentChunkSize;

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
    size_t getNextElement(T& dest);

    /**
     * Reads the next chunk and passes it to the pointer in dest
     * 
     * @param dest the destination pointer
     * 
     * @return the number of bytes read or -1 if something went wrong
     */
    size_t getNextChunk(T*& dest);

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

template<typename T>
inputBuffer<T>::inputBuffer( int fd, uint64_t bufferSize ) : 
        _fd(fd), 
        _bufferSize(bufferSize), 
        _cur(-1),
        _currentChunkSize(-1) {
    
    _buffer = new T[bufferSize];
}

template<typename T>
inputBuffer<T>::~inputBuffer() {
    delete[] _buffer;
}

template<typename T>
size_t inputBuffer<T>::loadNextChunk() {
    _cur = 0;
    
    size_t readBytes = read( _fd, _buffer, _bufferSize * DATA_SIZE);
    _currentChunkSize = readBytes;
    
    return readBytes;
}

template<typename T>
size_t inputBuffer<T>::getNextElement(T& dest) {
    //load new chunk either when no chunk was loaded before (_cur==-1), or current chunk is at the end (_cur== _bufferSize))
    // return 1 when everything was okay
    
    if(_cur == -1 || _cur == _currentChunkSize) {
        size_t loadResult = loadNextChunk();
        
        if(loadResult <= 0) {
            return loadResult;
        }
    }
    
    dest = _buffer[_cur];
    _cur++;
    
    return 1;
}

template<typename T>
size_t inputBuffer<T>::getNextChunk(T*& dest) {
    size_t readBytes = loadNextChunk();
    
    dest = _buffer;
    
    return readBytes / DATA_SIZE;
}

template<typename T>
bool inputBuffer<T>::isEOF() {

}

template<typename T>
off64_t inputBuffer<T>::getFileSize() {
    
}

#endif	/* INPUTBUFFER_HPP */

