/* 
 * File:   EnrichedQueue.hpp
 * Author: johannes
 *
 * Created on May 3, 2013, 12:06 AM
 */

#ifndef ENRICHEDQUEUE_HPP
#define	ENRICHEDQUEUE_HPP

#include <list>
#include <algorithm>
#include <iterator>

namespace dbi {

    template <typename T>
    class EnrichedQueue {
    public:

        EnrichedQueue() {

        }

        virtual ~EnrichedQueue() {

        }

        void enqueue(T obj) {
            _data.push_back(obj);
        }

        T dequeue() {
            T first = _data.front();
            _data.remove(first);
            return first;
        }

        bool contains(T obj) {
            return std::find(_data.begin(), _data.end(), obj) != _data.end();
        }

        void remove(T obj) {
            if (contains(obj)) {
                _data.remove(obj);
            }
        }

        void reEnqueue(T obj) {
            remove(obj);
            enqueue(obj);
        }

        int size() {
            return _data.size();
        }

        T get(int index) {
            typename std::list<T>::iterator it = _data.begin();
            std::advance(it, index);
            return *it;
        }

    private:
        std::list<T> _data;
    };
}
#endif	/* ENRICHEDQUEUE_HPP */

