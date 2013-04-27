/* 
 * File:   Run.hpp
 * Author: alexx
 *
 * Created on 25. April 2013, 19:45
 */

#ifndef RUN_HPP
#define	RUN_HPP

#include <cstdint>

#include "file_descriptor.hpp"

namespace dbi {

    class RunDescriptor {
    private:
        
        static const uint32_t MAX_FILENAME_LENGTH = 128;

        const char* FILENAME_PREFIX = "run";

    public:
        RunDescriptor(uint64_t number);
        virtual ~RunDescriptor();

        int openForRead();
        int createAndOpen();

        int remove();

    private:

        char _fileName[MAX_FILENAME_LENGTH];

        uint64_t _number;

    };

}

#endif	/* RUN_HPP */

