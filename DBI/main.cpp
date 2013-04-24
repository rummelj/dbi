/* 
 * File:   main.cpp
 * Author: johannes
 *
 * Created on April 23, 2013, 3:57 PM
 */

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <new> 

#include "inputBuffer.h"

using namespace std;

struct sort_t {
    uint64_t value;
    uint64_t runNumber;
};

/**
 * 
 * @todo integrate in own class
 * 
 * @param fdInput
 * @param size
 * @param fdOutput
 * @param memSize
 */
void externalSort( int fdInput, uint64_t size, int fdOutput, uint64_t memSize ) {
    inputBuffer inBuf(fdInput, memSize);
    
    uint64_t* chunk_ptr;
    size_t readBytes;
    
    while( (readBytes = inBuf.getNextChunk(chunk_ptr)) > 0) {
        std::cout << "read: " << readBytes << std::endl;
        std::vector<uint64_t> data(chunk_ptr, chunk_ptr + readBytes);
        
        std::sort(data.begin(), data.end());
        
        for( uint64_t& v : data ) {
            std::cout << v << std::endl;
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


int main( int argc, char** argv )
{
	if(argc != 4) {
		cerr << "usage: " << argv[0] << " <input file> <output file> <memSize in MB>" << endl;
		return EXIT_FAILURE;
	}

	uint64_t memSize = atoi(argv[3]);// * 1024 * 1024;
        
        //TODO open input/output file, aquire filesize
        
        int fd = open( argv[1], O_RDONLY );
        //TODO error checking and RAII maybe
	
	externalSort( fd, 0, 0, memSize ); 
        
        close(fd);

	return EXIT_SUCCESS;
}

