/* 
 * File:   mutex_holder.hpp
 * Author: alexx
 *
 * Created on 18. Mai 2013, 14:48
 */

#ifndef MUTEX_LOCK_HPP
#define	MUTEX_LOCK_HPP

#include <pthread.h>
#include <glog/logging.h>

class mutex_lock_holder {
    
    pthread_mutex_t& _mutex;
    
public:
    
    mutex_lock_holder(pthread_mutex_t& mutex) : _mutex(mutex) {
        LOG(INFO) << "Locking mutex: " << &_mutex;
        pthread_mutex_lock(&_mutex);
    }
    
    ~mutex_lock_holder() {
        LOG(INFO) << "Unlocking mutex: " << &_mutex;
        pthread_mutex_unlock(&_mutex);
    }
    
};



#endif	/* MUTEX_LOCK_HPP */

