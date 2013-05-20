/* 
 * File:   SegmentManager.hpp
 * Author: alexx
 *
 * Created on 20. Mai 2013, 11:27
 */

#ifndef SEGMENTMANAGER_HPP
#define	SEGMENTMANAGER_HPP

class FifoStrategy;
class PageFileManager;

template class BufferManager<FifoStrategy, PageFileManager>;



class SegmentManager {
    
    typedef BufferManager<FifoStrategy, PageFileManager> BufferManager_t;
    
public:
    SegmentManager(BufferManager_t& bufferManager);
    SegmentManager(const SegmentManager& orig);
    virtual ~SegmentManager();
private:
    
    BufferManager_t& _bufferManager;
    
    

};

#endif	/* SEGMENTMANAGER_HPP */

