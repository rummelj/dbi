/* 
 * File:   BufferManager.hpp
 * Author: johannes
 *
 * Created on May 2, 2013, 4:32 PM
 */

#ifndef BUFFERMANAGER_HPP
#define	BUFFERMANAGER_HPP

#include <string>
#include <map>

#include "BufferFrame.hpp"
#include "PageFileManager.hpp"

using namespace std;

namespace dbi {
    namespace bm {

        class BufferManager {
        public:
            /**
             * Create a new instance that manages size frames and operates 
             * on the file filename
             * @param filename
             * @param size
             */
            BufferManager(const std::string& filename, uint64_t size);
            
            /**
             * A method to retrieve frames given a page ID and indicating 
             * whether the page will be held exclusively by this thread or not.
             * The method can fail if no free frame is available and no used 
             * frame can be freed.
             * @param pageId
             * @param exclusive
             * @return 
             */
            BufferFrame& fixPage(uint64_t pageId, bool exclusive);
            
            /**
             * Return a frame to the buffer manager indicating whether it is 
             * dirty or not. If dirty, the page manager must write it back to 
             * disk. It does not have to write it back immediately, but must 
             * not write it back before unfixPage is called.
             * @param frame
             * @param isDirty
             */
            void unfixPage(BufferFrame& frame, bool isDirty);

            /**
             * Write all dirty frames to disk and free all resources.
             */
            virtual ~BufferManager();
            
        protected:
            uint64_t _size;
            PageFileManager _pageFileManager;
            
            
            
        private:
            map<uint64_t, BufferFrame*> _buffer;
            
            bool isInBuffer(uint64_t pageId);
            BufferFrame& getFromBuffer(uint64_t pageId);
            void saveInBuffer(uint64_t pageId, BufferFrame& bufferFrame);
        };

    }
}
#endif	/* BUFFERMANAGER_HPP */

