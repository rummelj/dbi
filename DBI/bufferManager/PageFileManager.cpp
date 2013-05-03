/* 
 * File:   PageFileManager.cpp
 * Author: johannes
 * 
 * Created on May 2, 2013, 5:31 PM
 */

#include <string>

#include "PageFileManager.hpp"

namespace dbi {

    PageFileManager::PageFileManager(const std::string& filename) {

    }

    PageFileManager::~PageFileManager() {

    }

    void* PageFileManager::readPage(uint64_t pageId) {
        return nullptr;
    }

    void PageFileManager::createPage(uint64_t pageId) {

    }

    void PageFileManager::writePage(uint64_t pageId, void* data) {

    }

    bool PageFileManager::isExisting(uint64_t pageId) {
        return false;
    }

    uint64_t* getPages() {
        return nullptr;
    }
}

