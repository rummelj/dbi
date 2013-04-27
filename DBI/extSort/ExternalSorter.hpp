/* 
 * File:   ExternalSorter.hpp
 * Author: alexx
 *
 * Created on 25. April 2013, 18:40
 */

#ifndef EXTERNALSORTER_HPP
#define	EXTERNALSORTER_HPP

#include "InputBuffer.hpp"

#include <vector>

namespace dbi {

    class RunDescriptor;
    
    class ExternalSorter {
    public:
        ExternalSorter(const char* fileInput, const char* fileOutput, uint64_t memSize);
        virtual ~ExternalSorter();

        void sort();

    private:


        const char* _fileInput;
        const char* _fileOutput;

        uint64_t _memSize;


        void externalSort(int fdInput, uint64_t size, int fdOutput);

        RunDescriptor* sortChunk(uint64_t* chunk_ptr, const size_t readElements, const uint64_t runNumber);

        std::vector<RunDescriptor*>* readChunks(const int fdInput, const uint64_t size);

        void mergeRuns( std::vector<RunDescriptor*>& runDescriptors, const int fdOutput );
        
    };

}

#endif	/* EXTERNALSORTER_HPP */

