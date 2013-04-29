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
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>

namespace dbi {

    template<typename T>
    class InputBuffer {
    public:
        InputBuffer(int fd, uint64_t bufferSize);

        virtual ~InputBuffer() {
        }

    protected:

        const size_t DATA_SIZE = sizeof (T);

        int _fd; // fd = file descriptor, quasi ein zeiger auf eine datei
        uint64_t _bufferSize; // größe des buffers ( _buffer wird dann _bufferSize Ts enthalten können)

        char* _buffer;

        off_t _cur;
        size_t _currentChunkSize;

        /**
         * reads (max) bufferSize 'T's into the buffer and returns the actual number of bytes read
         * 
         * @return the number of bytes read
         */
        virtual size_t loadNextChunk() = 0;

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
    class ArrayInputBuffer : public InputBuffer<T> {
    public:

        ArrayInputBuffer(int fd, uint64_t bufferSize);

        virtual ~ArrayInputBuffer();

    protected:

        size_t loadNextChunk();

    };

    template<typename T>
    class MMapInputBuffer : public InputBuffer<T> {
    public:


        MMapInputBuffer(int fd, uint64_t bufferSize);

        virtual ~MMapInputBuffer();

    private:
        const long PAGE_SIZE;

        off_t _offset;

        off_t _file_size;

    protected:

        size_t loadNextChunk();

    };

    template<typename T>
    InputBuffer<T>::InputBuffer(int fd, uint64_t bufferSize) :
    _fd(fd),
    _bufferSize(bufferSize),
    _buffer(NULL),
    _cur(-1),
    _currentChunkSize(-1) {

    }

    template<typename T>
    size_t InputBuffer<T>::getNextElement(T& dest) {
        //load new chunk either when no chunk was loaded before (_cur==-1), or current chunk is at the end (_cur== _bufferSize))
        // return 1 when everything was okay

        if (_cur == -1 || (size_t)_cur == _currentChunkSize) {
            size_t loadResult = loadNextChunk();

            if (loadResult <= 0) {
                return loadResult;
            }
        }

        memcpy(&dest, _buffer + _cur, DATA_SIZE);

        _cur += DATA_SIZE;

        return 1;
    }

    template<typename T>
    size_t InputBuffer<T>::getNextChunk(T*& dest) {
        size_t readBytes = loadNextChunk();

        dest = (T*) _buffer;

        return readBytes / DATA_SIZE;
    }

    template<typename T>
    int InputBuffer<T>::getFd() {
        return _fd;
    }

    template<typename T>
    ArrayInputBuffer<T>::ArrayInputBuffer(int fd, uint64_t bufferSize) : InputBuffer<T>(fd, bufferSize) {
        this->_buffer = new char[bufferSize];
    }

    template<typename T>
    ArrayInputBuffer<T>::~ArrayInputBuffer() {
        delete[] this->_buffer;
    }

    template<typename T>
    size_t ArrayInputBuffer<T>::loadNextChunk() {
        this->_cur = 0;

        size_t readBytes = read(this->_fd, this->_buffer, this->_bufferSize);
        this->_currentChunkSize = readBytes;

        return readBytes;
    }

    template<typename T>
    MMapInputBuffer<T>::MMapInputBuffer(int fd, uint64_t bufferSize)
    : InputBuffer<T>(fd, bufferSize),
    PAGE_SIZE(sysconf(_SC_PAGESIZE)),
    _offset(0) {

        assert((bufferSize / PAGE_SIZE) * PAGE_SIZE == bufferSize); // bufferSize has to be multiple of the pagesize

        struct stat statData;

        if (fstat(fd, &statData) < 0) {
            perror("fstat");
            assert(false);
        }

        _file_size = statData.st_size;
    }

    template<typename T>
    MMapInputBuffer<T>::~MMapInputBuffer() {

        if (this->_buffer != NULL) {
            munmap(this->_buffer, this->_currentChunkSize);
        }

    }

    template<typename T>
    size_t MMapInputBuffer<T>::loadNextChunk() {
        this->_cur = 0;

        if (this->_buffer != NULL) {
            munmap(this->_buffer, this->_currentChunkSize);
        }

        if (_offset >= _file_size) {
            return 0;
        }

        this->_buffer = (char*) mmap(NULL, this->_bufferSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, this->_fd, _offset);

        if (this->_buffer == MAP_FAILED) {
            perror("mmap");
            assert(false);
        }

        _offset += this->_bufferSize;

        this->_currentChunkSize = this->_bufferSize;
        return this->_currentChunkSize;
    }




}



#endif	/* INPUTBUFFER_HPP */

