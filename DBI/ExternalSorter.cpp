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

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>


#include "ExternalSorter.hpp"
#include "OutputBuffer.hpp"
#include "InputBuffer.hpp"
#include "file_descriptor.hpp"
#include "RunDescriptor.hpp"

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

    file_descriptor fdInput(open(_fileInput, O_RDONLY));
    file_descriptor fdOutput(open(_fileOutput, O_WRONLY | O_CREAT, S_IROTH | S_IRGRP | S_IRUSR | S_IWUSR)); //files will be closed when file_descriptor gets deleted

    uint64_t numberCount = fdInput.getSize() / sizeof (uint64_t);
    
    

    externalSort(fdInput, numberCount, fdOutput);

}

uint64_t ExternalSorter::readChunks(const int fdInput, const uint64_t size) {

    InputBuffer<uint64_t> inBuf(fdInput, _memSize);

    uint64_t* chunk_ptr;
    size_t readElements;

    uint64_t numRuns = ceil_divide(size * sizeof(uint64_t), _memSize);

    uint64_t runNumber = 0;

    while ((readElements = inBuf.getNextChunk(chunk_ptr)) > 0) {
        //std::cout << "read: " << readElements << std::endl;

        RunDescriptor* runDescriptor = sortChunk(chunk_ptr, readElements, runNumber);
        _runDescriptors.push_back(runDescriptor);
        
        runNumber++;
    }
    assert(runNumber == numRuns); //actual number of runs == projected number of runs

    return numRuns;
}

/**
 * 
 * 
 * @param fdInput
 * @param size
 * @param fdOutput
 * @param memSize
 */
void ExternalSorter::externalSort(int fdInput, uint64_t size, int fdOutput) {

    _runDescriptors.clear();
    
    uint64_t numRuns = readChunks( fdInput, size );
    
    uint64_t memoryPerRun = ((_memSize / (numRuns+1)) / sizeof(uint64_t)) * sizeof(uint64_t); //TODO numRuns + 1 might be > _memSize
    
    //std::cout << "merge " << memoryPerRun << std::endl;

    std::vector<InputBuffer<uint64_t>*> runInputBuffers;
    
    for( RunDescriptor*& descriptor : _runDescriptors ) {
        runInputBuffers.push_back( new InputBuffer<uint64_t>( descriptor->openForRead(), memoryPerRun )  );
    }
    
    OutputBuffer<uint64_t> outBuf( fdOutput, memoryPerRun );
    
    std::priority_queue<sort_t, std::vector<sort_t>, sort_t_order> sortQueue;
    
    for( InputBuffer<uint64_t>*& inBuf : runInputBuffers ) {
        uint64_t readInt;
        size_t readBytes = inBuf->getNextElement(readInt);
        
        //std::cout << readInt << std::endl;

        assert( readBytes == 1 );
        
        sortQueue.push( { readInt, inBuf } );
    }
    
    while(!sortQueue.empty()) {
        
        sort_t sortedElement = sortQueue.top();
        sortQueue.pop();
        
        outBuf.writeElement( sortedElement.value );
        
        //std::cout << "top:" << sortedElement.value << std::endl;
        
        size_t readBytes = sortedElement.runBuffer->getNextElement( sortedElement.value );
        
        if(readBytes == 1) {
            sortQueue.push( sortedElement );
        }
    }
    
    for( InputBuffer<uint64_t>*& inBuf : runInputBuffers ) {
        close(inBuf->getFd());
        delete inBuf;
    }
    
    for( RunDescriptor*& descriptor : _runDescriptors ) {
        descriptor->remove();
        delete descriptor;
    }

}

RunDescriptor* ExternalSorter::sortChunk(uint64_t* chunk_ptr, const size_t readElements, const uint64_t runNumber) {

    RunDescriptor* newRun = new RunDescriptor( runNumber );
    
    std::vector<uint64_t> data(chunk_ptr, chunk_ptr + readElements);

    std::sort(data.begin(), data.end());
    
    /*
    for( uint64_t& i : data ) {
        std::cout << i << std::endl;
    }
    */
    
    file_descriptor runFd( newRun->createAndOpen() );

    OutputBuffer<uint64_t> outBuf(runFd, _memSize);
    outBuf.writeChunk(&data[0], readElements);
    
    return newRun;
}

