/* 
 * File:   SortQueue.hpp
 * Author: alexx
 *
 * Created on 27. April 2013, 16:55
 */

#ifndef SORTQUEUE_HPP
#define	SORTQUEUE_HPP

#include "InputBuffer.hpp"
#include "OutputBuffer.hpp"

#include <vector>
#include <queue>

namespace dbi {

    class SortQueue {

        struct sort_t {
            uint64_t value;
            InputBuffer<uint64_t>* runBuffer;
        };

        class sort_t_order {
        public:

            bool operator()(const sort_t* s1, const sort_t* s2) {
                return s1->value > s2->value;
            }

        };

        typedef std::priority_queue<sort_t*, std::vector<sort_t*>, sort_t_order> queue_t;


    public:
        SortQueue(std::vector<InputBuffer<uint64_t>*>& runInputBuffers, OutputBuffer<uint64_t>& outBuf );
    private:
        
        std::vector<InputBuffer<uint64_t>*>& _runInputBuffers;
        OutputBuffer<uint64_t>& _outBuf;
        
        queue_t _sortQueue;

        void initialize( );

    public:

        void sort();

    };

}

#endif	/* SORTQUEUE_HPP */

