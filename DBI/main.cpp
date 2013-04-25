/* 
 * File:   main.cpp
 * Author: johannes
 *
 * Created on April 23, 2013, 3:57 PM
 */

#include <cstdlib>
#include <cstdint>

#include <iostream>

#include "ExternalSorter.hpp"

using namespace std;
using namespace dbi;


int main( int argc, char** argv )
{
	if(argc != 4) {
		cerr << "usage: " << argv[0] << " <input file> <output file> <memSize in MB>" << endl;
		return EXIT_FAILURE;
	}

	uint64_t memSize = atoi(argv[3]); // * 1024 * 1024;
        
        ExternalSorter sorter( argv[1], argv[2], memSize );
        
        sorter.sort();
        
        //TODO open input/output file, aquire filesize
                
        //int fd = open( argv[1], O_RDONLY );
        //TODO error checking and RAII maybe
	
	//externalSort( fd, 0, 0, memSize ); 
        
        //close(fd);

	return EXIT_SUCCESS;
}

