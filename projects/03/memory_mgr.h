#pragma once

#include <cstdlib>

namespace MemoryMgr {
    void* allocate(std::size_t size);
    void deallocate(void* ptr);
}
