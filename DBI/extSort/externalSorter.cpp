/* 
 * File:   externalSort.cpp
 * Author: johannes
 * 
 * Created on April 25, 2013, 9:49 AM
 */

using namespace std;

#include "inputBuffer.h"
#include "externalSorter.h"
#include <iostream>
#include <vector>
#include <algorithm>

struct sort_t {
    uint64_t value;
    uint64_t runNumber;
};

externalSorter::externalSorter() {
}

externalSorter::~externalSorter() {
}

/**
 * @param fdInput
 * @param size
 * @param fdOutput
 * @param memSize
 */
void externalSorter::externalSort( int fdInput, uint64_t size, int fdOutput, uint64_t memSize ) {
    inputBuffer inBuf(fdInput, memSize);
    
    uint64_t* chunk_ptr;
    size_t readBytes;
    
    while( (readBytes = inBuf.getNextChunk(chunk_ptr)) > 0) {
        cout << "read: " << readBytes << endl;
        vector<uint64_t> data(chunk_ptr, chunk_ptr + readBytes);
        
        sort(data.begin(), data.end());
        
        for( uint64_t& v : data ) {
            cout << v << endl;
        }
        
        //write data to temp-file
        //memorize which run is in which file & maximum count of files
        
    }
    
    //open inputbuffers for all tempfiles (buffer size 1?)
    //open outputbuffer for fdOutput ()
    
    //create priority queue for sort_t
    
    //go through all temp files, aquire element, insert in priority queue
    
    //unless no new element can be loaded
    //

    
}


