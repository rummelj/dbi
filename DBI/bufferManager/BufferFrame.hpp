/* 
 * File:   BufferFrame.hpp
 * Author: johannes
 *
 * Created on May 2, 2013, 4:42 PM
 */

#ifndef BUFFERFRAME_HPP
#define	BUFFERFRAME_HPP

namespace dbi {
    namespace bm {

        class BufferFrame {
        public:
            BufferFrame();
            BufferFrame(const BufferFrame& orig);
            virtual ~BufferFrame();
            
            /**
             * A buffer frame should offer a method giving access to the 
             * buffered page. Except for the buffered page, BufferFrame objects 
             * can also store control information (page ID, dirtyness, . . . ).
             * @return 
             */
            void* getData();
        private:

        };
    }
}

#endif	/* BUFFERFRAME_HPP */

