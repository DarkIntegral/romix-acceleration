#include <chrono>
#include <vector>

class RomixBase
{
public:
    virtual void mix(uint8_t* data, size_t blocks) = 0;

private:
    
    // BIP 38 romix constants
    const size_t mCostFactor = 16384;
    const size_t mBlockSizeFactor = 8;
    const size_t mBlockSize = 128 * mBlockSizeFactor;
    const size_t mDesiredKeyLen = 32;
    
    // Common timing and analytics
    void timer_start();
    void timer_end(size_t blocks);
    size_t get_total_blocks();
    size_t get_total_ns();
    double get_blocks_per_second();
    
    typedef std::chrono::high_resolution_clock mClock;
    unsigned long mTotalBlocks = 0;
    unsigned long mTotalNs = 0;
    std::chrono::time_point<mClock> mStartTime, mEndTime
};
