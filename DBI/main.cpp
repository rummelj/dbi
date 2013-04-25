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

#include "extSort/inputBuffer.h"
#include "extSort/externalSorter.h"

using namespace std;

int main( int argc, char** argv )
{
	if(argc != 4) {
		cerr << "usage: " << argv[0] << " <input file> <output file> <memSize in MB>" << endl;
		return EXIT_FAILURE;
	}

	uint64_t memSize = atoi(argv[3]);// * 1024 * 1024;
        
        externalSorter* sorter = new externalSorter();
        
        //TODO open input/output file, aquire filesize
        
        int fd = open( argv[1], O_RDONLY );
        //TODO error checking and RAII may
        
	sorter->externalSort( fd, 0, 0, memSize ); 
        
        close(fd);

	return EXIT_SUCCESS;
}

