/* 
 * File:   PageFileManager.cpp
 * Author: johannes
 * 
 * Created on May 2, 2013, 5:31 PM
 */

#include <pthread.h>

#include "TwoQStrategy.hpp"
#include "EnrichedQueue.hpp"

namespace dbi {

    TwoQStrategy::TwoQStrategy() {
        pthread_mutex_init(&_preserved_mutex, NULL);
        pthread_mutex_init(&_fifo_mutex, NULL);
        pthread_mutex_init(&_lru_mutex, NULL);
    }

    TwoQStrategy::~TwoQStrategy() {
        pthread_mutex_destroy(&_preserved_mutex);
        pthread_mutex_destroy(&_fifo_mutex);
        pthread_mutex_destroy(&_lru_mutex);
    }

    void TwoQStrategy::know(uint64_t id) {
        lockFifo();
        lockPreserved();
        //Put in fifo
        if (!_fifo.contains(id) && !_lru.contains(id)) {
            _fifo.enqueue(id);
        }

        // Initially not preserved
        if (_preserved.count(id) == 0) {
            _preserved[id] = false;
        }
        unlockPreserved();
        unlockFifo();
    }

    void TwoQStrategy::forget(uint64_t id) {
        lockFifo();
        //Remove from fifo
        if (_fifo.contains(id)) {
            _fifo.remove(id);
        }
        unlockFifo();

        lockLru();
        //And or from lru
        if (_lru.contains(id)) {
            _lru.remove(id);
        }
        unlockLru();
    }

    void TwoQStrategy::used(uint64_t id) {
        lockFifo();
        lockLru();

        //If already in lru put it at the end
        if (_lru.contains(id)) {
            _lru.reEnqueue(id);
        }

        //If "only" in fifo move it to lru
        if (_fifo.contains(id)) {
            _fifo.remove(id);
            _lru.enqueue(id);
        }

        // If not known at all til now, know it (means add it to fifo)
        if (!_fifo.contains(id) && !_lru.contains(id)) {
            _fifo.enqueue(id);
        }

        unlockLru();
        unlockFifo();
    }

    void TwoQStrategy::preserve(uint64_t id) {
        lockPreserved();
        if (_preserved[id]) {
            pthread_cond_wait(&_preserved_changed, &_preserved_mutex);
            unlockPreserved();
            preserve(id);
            return;
        }
        _preserved[id] = true;
        unlockPreserved();
    }

    void TwoQStrategy::release(uint64_t id) {
        lockPreserved();
        _preserved[id] = false;
        unlockPreserved();
        pthread_cond_signal(&_preserved_changed);
    }

    uint64_t TwoQStrategy::getEvictable() {
        lockFifo();
        //Search in fifo first
        for (int i = 0; i < _fifo.size(); i++) {
            uint64_t id = _fifo.get(i);
            if (!_preserved[id]) {
                return id;
            }
        }
        unlockFifo();

        lockLru();
        //Search in lru after
        for (int i = 0; i < _lru.size(); i++) {
            uint64_t id = _lru.get(i);
            if (!_preserved[id]) {
                return id;
            }
        }
        unlockLru();

        //Nothing found
        return 0;
    }

    unsigned int TwoQStrategy::size() {
        lockFifo();
        lockLru();
        return _fifo.size() + _lru.size();
        unlockLru();
        unlockFifo();
    }

    void TwoQStrategy::setPreserved(uint64_t id, bool value) {
        lockPreserved();
        _preserved[id] = value;
        unlockPreserved();
    }

    void TwoQStrategy::lockPreserved() {
        pthread_mutex_lock(&_preserved_mutex);
    }

    void TwoQStrategy::unlockPreserved() {
        pthread_mutex_unlock(&_preserved_mutex);
    }

    void TwoQStrategy::lockFifo() {
        pthread_mutex_lock(&_fifo_mutex);
    }

    void TwoQStrategy::unlockFifo() {
        pthread_mutex_unlock(&_fifo_mutex);
    }

    void TwoQStrategy::lockLru() {
        pthread_mutex_lock(&_lru_mutex);
    }

    void TwoQStrategy::unlockLru() {
        pthread_mutex_unlock(&_lru_mutex);
    }
}
