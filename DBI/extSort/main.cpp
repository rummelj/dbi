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

	uint64_t memSize = atoi(argv[3]) * 1024 * 1024;
        
        ExternalSorter sorter( argv[1], argv[2], memSize );
        
        sorter.sort();

	return EXIT_SUCCESS;
}

