/* 
 * File:   BufferFrame.hpp
 * Author: johannes
 *
 * Created on May 2, 2013, 4:42 PM
 */

#ifndef BUFFERFRAME_HPP
#define	BUFFERFRAME_HPP

#include <cstdint>

namespace dbi {

    class BufferFrame {

    public:
        BufferFrame(uint64_t pageId, void* data);
        virtual ~BufferFrame();

    private: //don't allow copying of bufferframes
        BufferFrame( const BufferFrame& copy );
        BufferFrame& operator= (BufferFrame const& rhs);
        
    public:
        
        enum LockIntend {
            Intend_Exclusive,
            Intend_Shared
        };
        
        enum LockState {
            State_Exclusive = LockIntend::Intend_Exclusive,
            State_Shared = LockIntend::Intend_Shared,
            State_None
        };

        /**
         * A buffer frame should offer a method giving access to the 
         * buffered page. Except for the buffered page, BufferFrame objects 
         * can also store control information (page ID, dirtyness, . . . ).
         * @return 
         */
        void* getData();
        
        uint64_t lock(LockIntend lockIntend);
        uint64_t removeLock(bool isDirty);
        
        bool isLocked();
        bool isDirty();
        
        uint64_t getPageId();
        LockState getLockState();

    private:
        uint64_t _pageId;
        void* _data;
        bool _exclusive;
        bool _dirty;
        LockState _lockState;
        uint64_t _fixCount;
        
        pthread_mutex_t _exclusive_mutex;
        pthread_cond_t _exclusive_changed = PTHREAD_COND_INITIALIZER;
    };
}

#endif	/* BUFFERFRAME_HPP */

