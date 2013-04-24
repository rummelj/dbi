/* 
 * File:   main.cpp
 * Author: johannes
 *
 * Created on April 23, 2013, 3:57 PM
 */

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <new> 

#include "inputBuffer.hpp"
#include "outputBuffer.hpp"

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
    inputBuffer<uint64_t> inBuf(fdInput, memSize);
    
    uint64_t* chunk_ptr;
    size_t readElements;
    
    //uint64_t numRuns = memSize / size;
    uint64_t runNumber = 0;
    
    //do sth when numRuns > memSize
   
    while( (readElements = inBuf.getNextChunk(chunk_ptr)) > 0) {
        std::cout << "read: " << readElements << std::endl;
        std::vector<uint64_t> data(chunk_ptr, chunk_ptr + readElements);
        
        std::sort(data.begin(), data.end());
        
        
        
        for( uint64_t& v : data ) {
            std::cout << v << std::endl;
        }
        
        std::stringstream sstream("run");
        sstream << runNumber;
        
        const std::string runFilename( sstream.str() );
        
        int fd = open( runFilename.c_str(), O_WRONLY | O_CREAT, S_IROTH | S_IRGRP | S_IWUSR | S_IRUSR );
        
        outputBuffer<uint64_t> outBuf( fd, memSize );
        
        outBuf.writeChunk( &data[0], readElements );
        outBuf.flush();
        
        close(fd);
        
        runNumber ++;
        
        //write data to temp-file
        //memorize maximum count of files
        
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

