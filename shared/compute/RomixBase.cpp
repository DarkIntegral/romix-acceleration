#include "RomixBase.hpp"

RomixBase::RomixBase() {}

/**
 Indicate the start of a timing block.
 */
void RomixBase::timer_start() { mStartTime = mClock::now(); }

/**
 Indicates the end of a timing block.
 @param blocks The number of blocks that were mixed during this time.
 */
void RomixBase::timer_end(size_t blocks) {
  using namespace std::chrono;
  mTotalNs += duration_cast<nanoseconds>(mClock::now() - mStartTime).count();
  mTotalBlocks += blocks;
}

/**
 Get the total number of ns the romix object was in timing mode.
 @return Number of ns that were timed.
 */
size_t RomixBase::get_total_blocks() { return mTotalBlocks; }

/**
 Get the total number of ns the romix object was in timing mode.
 @return Number of ns that were timed.
 */
size_t RomixBase::get_total_ns() { return mTotalNs; }

/**
 Generate the number of Romix blocks mixed per second.
 @return Blocks mixed per second.
 */
double RomixBase::get_blocks_per_second() {
  // sanity check something has been timed.
  if (ns == 0)
    return 0;

  // compute number of blocks mixed per second.
  double seconds = ns / 1.0e9;
  double bbs = mTotalBlocks / seconds;
  return bbs;
}
