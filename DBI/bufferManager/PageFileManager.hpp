/* 
 * File:   PageFileManager.hpp
 * Author: johannes
 *
 * Created on May 2, 2013, 5:31 PM
 */

#ifndef PAGEFILEMANAGER_HPP
#define	PAGEFILEMANAGER_HPP

#include <string>
#include <unistd.h>

namespace dbi {

    class PageFileManager {
    public:
        PageFileManager(const std::string& filename);
        virtual ~PageFileManager();

        /**
         * Reads a page from disk.
         * @param pageId
         * Id of the page
         * @return 
         * data stored on the page.
         */
        void* readPage(uint64_t pageId);

        /**
         * Creates a new page with empty data.
         * @param pageId
         * id of the new page
         * @return 
         */
        void createPage(uint64_t pageId);

        /**
         * Writes a new page to disk or overwrites an existing page
         * @param pageId
         * Existing pageId or new pageId
         * @param data
         * data to be stored in the page
         */
        void writePage(uint64_t pageId, void* data);

        /**
         * Checks if the page with the given id is existing.
         * @param pageId
         * @return 
         */
        bool isExisting(uint64_t pageId);

        /**
         * Gets all page ids available
         * @return 
         * An array of all pages available on disk
         */
        uint64_t* getPages();
        
        /**
         * Closes a page and frees the buffer memory taken for this page
         */
        void closePage(uint64_t pageId, void* data);
    private:

	static const mode_t FILE_CREATE_FLAGS = 0644; // User ReadWrite, Group Read, Others Read
	static const uint8_t PAGE_MULTIPLIER = 1;
	const long OS_PAGE_SIZE ;

	const std::string& _filename;
	
	const size_t _pageSize;

	int _fd;

	off_t _fileSize;


	void recalculateFilesize();

	

    };
}
#endif	/* PAGEFILEMANAGER_HPP */

