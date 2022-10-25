#include "memory_mgr.h"

#include <new>

namespace MemoryMgr {

void* allocate(std::size_t size) {
    void* p = std::malloc(size);

    if (!p)
        throw std::bad_alloc();

    return p;
}

void deallocate(void* ptr) {
    std::free(ptr);
}

}
