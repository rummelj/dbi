/* 
 * File:   externalSort.h
 * Author: johannes
 *
 * Created on April 25, 2013, 9:49 AM
 */

#include "inputBuffer.h"
#include "outputBuffer.h"

#ifndef EXTERNALSORT_H
#define	EXTERNALSORT_H

class externalSorter {
public:
    externalSorter();
    virtual ~externalSorter();
    
    void externalSort(int fdInput, uint64_t size, int fdOutput, uint64_t memSize);
private:
    int _fdInput;
    uint64_t _size;
    int _fdOutput;
    uint64_t _memsize;
};

#endif	/* EXTERNALSORT_H */

