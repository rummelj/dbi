/* 
 * File:   BufferFrame.cpp
 * Author: johannes
 * 
 * Created on May 2, 2013, 4:42 PM
 */

#include <stdlib.h>
#include <pthread.h>

#include <iostream>

#include <glog/logging.h>
#include <cassert>

#include "BufferFrame.hpp"
#include "mutex_lock.hpp"

namespace dbi {

    BufferFrame::BufferFrame(uint64_t pageId, void* data) :
    _pageId(pageId),
    _data(data),
    _exclusive(false),
    _dirty(false),
    _lockState(LockState::State_None),
    _fixCount(0) {

        pthread_rwlock_init(&_rwlock, NULL);
        pthread_mutex_init(&_metadata_mutex, NULL);

        LOG(INFO) << "BufferFrame created for page #" << pageId << std::endl;
    }

    BufferFrame::BufferFrame(const BufferFrame& copy) {
        LOG(FATAL) << "Don't copy BufferFrames.";
        assert(false);

    }

    BufferFrame& BufferFrame::operator=(BufferFrame const& copy) {
        LOG(FATAL) << "Don't copy BufferFrames.";
        assert(false);
        return *this;
    }

    BufferFrame::~BufferFrame() {
        LOG(INFO) << "BufferFrame destroyed for page #" << _pageId << std::endl;

        pthread_mutex_destroy(&_metadata_mutex);
        pthread_rwlock_destroy(&_rwlock);
    }

    void* BufferFrame::getData() {
        return _data;
    }

    uint64_t BufferFrame::lock(LockIntend lockIntend) {

        LOG(INFO) << "Locking Page #" << _pageId << ", intend: " << lockIntend;

        switch (lockIntend) {
            case LockIntend::Intend_Exclusive:
            {

                LOG(INFO) << "Requesting wrlock for Page #" << _pageId;
                pthread_rwlock_wrlock(&_rwlock);
                LOG(INFO) << "Wrlock received for Page #" << _pageId;

                _lockState = LockState::State_Exclusive;
                _fixCount = 1;

            }
            break;
            case LockIntend::Intend_Shared:
            {

                LOG(INFO) << "Requesting rdlock for Page #" << _pageId;
                pthread_rwlock_rdlock(&_rwlock);
                LOG(INFO) << "Requesting rdlock for Page #" << _pageId;

                _lockState = LockState::State_Shared;
                _fixCount++;

            }
            break;
        }

        return _fixCount;
    }

    uint64_t BufferFrame::release(bool isDirty) {
        LOG(INFO) << "Removing Lock Page " << _pageId << ", dirty: " << (isDirty ? "t" : "f");

        if (isDirty && _lockState != LockState::State_Exclusive) {
            LOG(FATAL) << "Cannot make a frame dirty that was not held exclusively.";
            assert(false);
        }

        switch (_lockState) {
            case LockState::State_Exclusive:
            {
                _dirty = isDirty;
                _fixCount = 0;
            }
            break;
            case LockState::State_Shared:
            {
                _fixCount--;
            }
            break;
            default:
            {
                LOG(FATAL) << "Trying to remove notexistant lock.";
                assert(false);
            }
            break;
        }

        if (_fixCount == 0) {
            _lockState = LockState::State_None;
            LOG(INFO) << "Releasing rwlock from Page #" << _pageId;
            pthread_rwlock_unlock(&_rwlock);
        }

        return _fixCount;
    }

    uint64_t BufferFrame::getPageId() {
        return _pageId;
    }

    BufferFrame::LockState BufferFrame::getLockState() {
        return _lockState;
    }

    bool BufferFrame::isLocked() {
        return _lockState != LockState::State_None;
    }

    bool BufferFrame::isDirty() {
        return _dirty;
    }

}

