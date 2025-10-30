
#pragma once
#include <atomic>
#include <cstddef>
#include <cstdlib>

class MemoryTracker
{
public:
    static size_t totalAllocated;
    static size_t peakAllocated;
    static size_t allocCount;
    static size_t deallocCount;
};
