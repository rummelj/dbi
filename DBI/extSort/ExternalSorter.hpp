/* 
 * File:   ExternalSorter.hpp
 * Author: alexx
 *
 * Created on 25. April 2013, 18:40
 */

#ifndef EXTERNALSORTER_HPP
#define	EXTERNALSORTER_HPP

#include "RunDescriptor.hpp"
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

        struct sort_t {
            uint64_t value;
            InputBuffer<uint64_t>* runBuffer;
        };
        
        class sort_t_order {
            
        public:
            
            bool operator()(const sort_t& s1, const sort_t& s2) {
                return s1.value > s2.value;
            }
            
        };

        std::vector<RunDescriptor*> _runDescriptors;

        const char* _fileInput;
        const char* _fileOutput;

        uint64_t _memSize;


        void externalSort(int fdInput, uint64_t size, int fdOutput);

        RunDescriptor* sortChunk(uint64_t* chunk_ptr, const size_t readElements, const uint64_t runNumber);

        uint64_t readChunks(const int fdInput, const uint64_t size);

    };

}

#endif	/* EXTERNALSORTER_HPP */

