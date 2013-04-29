/* 
 * File:   file_descriptor.hpp
 * Author: alexx
 *
 * Created on 25. April 2013, 09:05
 */

#ifndef FILE_DESCRIPTOR_HPP
#define	FILE_DESCRIPTOR_HPP

#include <cstdint>

namespace dbi {

class file_descriptor {
public:
    file_descriptor(int fd);
    virtual ~file_descriptor();
    
    operator const int& ();
    
    uint64_t getSize();
    
    void preallocate( uint64_t len, uint64_t offset = 0 );
    
private:
    
    int _fd;

    
};

}

#endif	/* FILE_DESCRIPTOR_HPP */

