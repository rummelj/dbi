/* 
 * File:   ExternalSorter.cpp
 * Author: alexx
 * 
 * Created on 25. April 2013, 18:40
 */

#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <memory>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>


#include "ExternalSorter.hpp"
#include "OutputBuffer.hpp"
#include "InputBuffer.hpp"
#include "file_descriptor.hpp"
#include "RunDescriptor.hpp"
#include "SortQueue.hpp"

using namespace dbi;

/**
 * Performs ceil(x/y) for any integer type
 * @param x denominator
 * @param y numerator
 * @return ceil(x/y)
 */
template<typename T>
inline T ceil_divide(T x, T y) {
    return 1 + ((x - 1) / y);
}

ExternalSorter::ExternalSorter(const char* fileInput, const char* fileOutput, uint64_t memSize) :
_fileInput(fileInput),
_fileOutput(fileOutput),
_memSize(memSize) {
}

ExternalSorter::~ExternalSorter() {
}

void ExternalSorter::sort() {

    file_descriptor fdInput( open(_fileInput, O_RDONLY) );
    file_descriptor fdOutput( open(_fileOutput, O_WRONLY | O_CREAT, S_IROTH | S_IRGRP | S_IRUSR | S_IWUSR) ); //files will be closed when file_descriptor gets deleted
    
    const uint64_t inputSize = fdInput.getSize();
    const uint64_t numberCount = inputSize / sizeof (uint64_t);
    
    fdOutput.preallocate( inputSize );

    externalSort(fdInput, numberCount, fdOutput);

}

void ExternalSorter::externalSort(int fdInput, uint64_t size, int fdOutput) const {

    std::unique_ptr<std::vector<RunDescriptor*>> runDescriptors( readChunks( fdInput, size ) );
    
    mergeRuns( *runDescriptors, fdOutput );
    
    for( RunDescriptor*& descriptor : *runDescriptors ) {
        descriptor->remove();
        delete descriptor;
    }

}

std::vector<RunDescriptor*>* ExternalSorter::readChunks(const int fdInput, const uint64_t size) const {

    ArrayInputBuffer<uint64_t> inBuf(fdInput, _memSize);
    
    std::vector<RunDescriptor*>* runDescriptors = new std::vector<RunDescriptor*>();
    
    uint64_t* chunk_ptr;
    size_t readElements;

    uint64_t numRuns = ceil_divide(size * sizeof(uint64_t), _memSize);

    uint64_t runNumber = 0;

    while ((readElements = inBuf.getNextChunk(chunk_ptr)) > 0) {
        //std::cout << "read: " << readElements << std::endl;

        RunDescriptor* runDescriptor = sortChunk(chunk_ptr, readElements, runNumber);
        runDescriptors->push_back(runDescriptor);
        
        runNumber++;
    }
    assert(runNumber == numRuns); //actual number of runs == projected number of runs

    return runDescriptors;
}

RunDescriptor* ExternalSorter::sortChunk(uint64_t* chunk_ptr, const size_t numElements, const uint64_t runNumber) const {

    std::sort(chunk_ptr, chunk_ptr + numElements);
    
    RunDescriptor* newRun = new RunDescriptor( runNumber );
    newRun->createAndWrite( chunk_ptr, numElements );

    return newRun;
}

void ExternalSorter::mergeRuns( std::vector<RunDescriptor*>& runDescriptors, const int fdOutput ) const {
    const uint64_t numRuns = runDescriptors.size(); 
    
    const uint64_t memoryPerRun = ((_memSize / (numRuns+1)) / sizeof(uint64_t)) * sizeof(uint64_t); //TODO numRuns + 1 might be > _memSize
    assert(memoryPerRun >= sizeof(uint64_t));
    
    std::vector<InputBuffer<uint64_t>*> runInputBuffers;
    
    for( RunDescriptor*& descriptor : runDescriptors ) {
        runInputBuffers.push_back( new ArrayInputBuffer<uint64_t>( descriptor->openForRead(), memoryPerRun )  );
    }
    
    OutputBuffer<uint64_t> outBuf( fdOutput, memoryPerRun );
    
    SortQueue sortQueue( runInputBuffers, outBuf );
    sortQueue.sort();
    
    for( InputBuffer<uint64_t>*& inBuf : runInputBuffers ) {
        close(inBuf->getFd());
        delete inBuf;
    }

}

