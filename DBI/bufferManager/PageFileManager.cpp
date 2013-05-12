/* 
 * File:   PageFileManager.cpp
 * Author: johannes
 * 
 * Created on May 2, 2013, 5:31 PM
 */

#include <cstdio>
#include <cassert>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "PageFileManager.hpp"

namespace dbi {

    PageFileManager::PageFileManager(const std::string& filename)
    : OS_PAGE_SIZE(sysconf(_SC_PAGESIZE))
    , _filename(filename)
    , _pageSize(OS_PAGE_SIZE * PAGE_MULTIPLIER) {

        //OS_PAGE_SIZE = sysconf( _SC_PAGESIZE );

        if (OS_PAGE_SIZE == -1) {
            perror("sysconf");

            //@todo
            ////throw 42;
            assert(false);
        }

        _fd = open(filename.c_str(), O_RDWR | O_CREAT, FILE_CREATE_FLAGS);

        if (_fd < 0) {
            perror("open");

            //@todo throw
            //throw 42;
            assert(false);
        }

        recalculateFilesize();
    }

    PageFileManager::~PageFileManager() {
        
        std::clog << "PageFileManager is being deleted" << std::endl;

        
        int status = close(_fd);

        if (status < 0) {
            perror("close");

            //@todo throw
            //throw 42;
            assert(false);
        }

    }

    void* PageFileManager::readPage(uint64_t pageId) {


        if (!isExisting(pageId)) {
            createPage(pageId);
        }

        const off_t offset = pageId * _pageSize;
        char* buffer = new char[_pageSize];

        if (lseek(_fd, offset, SEEK_SET) < 0) {
            perror("lseek");

            //@todo throw
            //throw 42;
            assert(false);
        }

        size_t readBytes = read(_fd, buffer, _pageSize);

        if (readBytes != _pageSize) {
            perror("read");

            fprintf(stderr, "readBytes: %lu pageSize: %lu\n", readBytes, _pageSize);

            //@todo throw
            //throw 42;
            assert(false);
        }

        return buffer;
    }

    void PageFileManager::createPage(uint64_t pageId) {

        const off_t offset = pageId * _pageSize;

        int statusAllocate = posix_fallocate(_fd, offset, _pageSize);

        if (statusAllocate != 0) {
            perror("posix_fallocate");

            //@todo throw
            //throw 42;
            assert(false);
        }

        char* buffer = new char[_pageSize]();

        if (lseek(_fd, offset, SEEK_SET) < 0) {
            perror("lseek");

            //@todo throw
            //throw 42;
            assert(false);
        }

        size_t wroteBytes = write(_fd, buffer, _pageSize);

        if (wroteBytes != _pageSize) {
            perror("write");

            //@todo throw
            //throw 42;
            assert(false);
        }

        delete[] buffer;

        recalculateFilesize();
    }

    void PageFileManager::writePage(uint64_t pageId, void* data) {

        if (!isExisting(pageId)) {
            createPage(pageId);
        }

        const off_t offset = pageId * _pageSize;

        if (lseek(_fd, offset, SEEK_SET) < 0) {
            perror("lseek");

            //@todo throw
            //throw 42;
            assert(false);
        }

        std::clog << "Writing page " << pageId << " data at " << data << " to file at pos " << offset << std::endl;
        
        size_t wroteBytes = write(_fd, data, _pageSize);

        if (wroteBytes != _pageSize) {
            perror("write");

            //@todo throw
            //throw 42;
            assert(false);
        }

        std::clog <<  "Deleting data at " << data << std::endl;
                
        if (data != NULL) {
            delete[] (char*) data; //converting necessary due to compiler
            data = NULL;
        }
        
    }

    bool PageFileManager::isExisting(uint64_t pageId) {
        return pageId * _pageSize < (size_t) _fileSize;
    }

    uint64_t* PageFileManager::getPages() {
        return nullptr;
    }

    void PageFileManager::recalculateFilesize() {

        struct stat buffer;

        int status = fstat(_fd, &buffer);

        if (status < 0) {
            perror("fstat");

            //@todo throw
            //throw 42;
            assert(false);
        }

        _fileSize = buffer.st_size;
    }

}

