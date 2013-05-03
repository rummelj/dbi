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
        friend class BufferManager;

    public:
        BufferFrame(void* data);
        virtual ~BufferFrame();

        /**
         * A buffer frame should offer a method giving access to the 
         * buffered page. Except for the buffered page, BufferFrame objects 
         * can also store control information (page ID, dirtyness, . . . ).
         * @return 
         */
        void* getData();

    private:
        uint64_t _pageId;
        void* _data;
        bool _exclusive;
        bool _dirty;
        int _fixCount;
        
        pthread_mutex_t _exclusive_mutex;
        pthread_cond_t _exclusive_changed = PTHREAD_COND_INITIALIZER;
    };
}

#endif	/* BUFFERFRAME_HPP */

