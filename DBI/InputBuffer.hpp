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
#include <cstring>

namespace dbi {

    template<typename T>
    class InputBuffer {
    public:
        InputBuffer(int fd, uint64_t bufferSize);
        virtual ~InputBuffer();

    private:

        const size_t DATA_SIZE = sizeof (T);

        int _fd; // fd = file descriptor, quasi ein zeiger auf eine datei
        uint64_t _bufferSize; // größe des buffers ( _buffer wird dann _bufferSize Ts enthalten können)

        char* _buffer;

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

        int getFd();

    };

    template<typename T>
    InputBuffer<T>::InputBuffer(int fd, uint64_t bufferSize) :
    _fd(fd),
    _bufferSize(bufferSize),
    _cur(-1),
    _currentChunkSize(-1) {

        _buffer = new char[bufferSize];
    }

    template<typename T>
    InputBuffer<T>::~InputBuffer() {
        delete[] _buffer;
    }

    template<typename T>
    size_t InputBuffer<T>::loadNextChunk() {
        _cur = 0;

        size_t readBytes = read(_fd, _buffer, _bufferSize );
        _currentChunkSize = readBytes;

        return readBytes;
    }

    template<typename T>
    size_t InputBuffer<T>::getNextElement(T& dest) {
        //load new chunk either when no chunk was loaded before (_cur==-1), or current chunk is at the end (_cur== _bufferSize))
        // return 1 when everything was okay

        if (_cur == -1 || _cur == _currentChunkSize) {
            size_t loadResult = loadNextChunk();

            if (loadResult <= 0) {
                return loadResult;
            }
        }
        
        memcpy( &dest, _buffer + _cur, DATA_SIZE);

        _cur += DATA_SIZE;

        return 1;
    }

    template<typename T>
    size_t InputBuffer<T>::getNextChunk(T*& dest) {
        size_t readBytes = loadNextChunk();

        dest = (T*)_buffer;

        return readBytes / DATA_SIZE;
    }
    
    template<typename T>
    int InputBuffer<T>::getFd() {
        return _fd;
    }

}

#endif	/* INPUTBUFFER_HPP */

