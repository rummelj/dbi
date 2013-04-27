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
    
private:
    
    int _fd;

    
};

}

#endif	/* FILE_DESCRIPTOR_HPP */

