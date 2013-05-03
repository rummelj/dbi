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
#include "InputBuffer.hpp"
#include "file_descriptor.hpp"

using namespace std;
using namespace dbi;

void testOutput(const char* outputFileName, const uint64_t memSize) {

    file_descriptor fd(open(outputFileName, O_RDONLY));

    ArrayInputBuffer<uint64_t> inBuf(fd, memSize);

    uint64_t value = 0;
    uint64_t oldValue = 0;

    size_t readBytes = inBuf.getNextElement(oldValue);
    assert(readBytes == 1);

    while (inBuf.getNextElement(value) == 1) {
        assert(value >= oldValue);
        oldValue = value;
    }

}

int main_old(int argc, char** argv) {
    if (argc != 4) {
        cerr << "usage: " << argv[0] << " <input file> <output file> <memSize in MB>" << endl;
        return EXIT_FAILURE;
    }

    uint64_t memSize = atoi(argv[3]) * 1024 * 1024; ///< convert thrid parameter to int and multiply by MB factors


    ExternalSorter sorter(argv[1], argv[2], memSize);

    std::cout << "Sorting data... ";

    sorter.sort();

    std::cout << " [OK]" << std::endl;

    std::cout << "Verifying sorted data... ";

    testOutput(argv[2], memSize);

    std::cout << " [OK] " << std::endl;

    std::cout << "Everything seems OK. Data hopefully sorted." << std::endl;

    return EXIT_SUCCESS;
}

