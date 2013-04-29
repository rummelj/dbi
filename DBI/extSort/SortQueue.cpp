/* 
 * File:   SortQueue.cpp
 * Author: alexx
 * 
 * Created on 27. April 2013, 16:55
 */

#include "SortQueue.hpp"

#include <cassert>

using namespace dbi;

SortQueue::SortQueue(std::vector<InputBuffer<uint64_t>*>& runInputBuffers, OutputBuffer<uint64_t>& outBuf)
: _runInputBuffers(runInputBuffers),
_outBuf(outBuf) {



}

void SortQueue::initialize() {

    for (InputBuffer<uint64_t>*& inBuf : _runInputBuffers) {
        uint64_t readInt;
        size_t readBytes = inBuf->getNextElement(readInt);

        //std::cout << readInt << std::endl;

        assert(readBytes == 1);

        _sortQueue.push(new sort_t({readInt, inBuf}));
    }
}

void SortQueue::sort() {

    initialize();
    
    

    while (!_sortQueue.empty()) {

        sort_t* sortedElement = _sortQueue.top();
        _sortQueue.pop();

        _outBuf.writeElement(sortedElement->value);

        size_t readBytes = sortedElement->runBuffer->getNextElement(sortedElement->value);

        if (readBytes == 1) {
            _sortQueue.push(sortedElement);
        } else {
            delete sortedElement;
        }
    }
}

