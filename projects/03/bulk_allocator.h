#pragma once

#include <list>
#include <algorithm>
#include "memory_block.h"

template <typename T, size_t N>
class BulkAllocator {
public:
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template <typename U>
    struct rebind {
        using other = BulkAllocator<U, N>;
    };

    BulkAllocator() = default;
    ~BulkAllocator() = default;

    template <typename U>
    BulkAllocator(const BulkAllocator<U, N>&) : memoryBlocks_() {}

    BulkAllocator(const BulkAllocator<T, N>&) : memoryBlocks_() {}

    T* allocate(std::size_t n) {
        T* p = nullptr;
        for (auto it = memoryBlocks_.begin(); it != memoryBlocks_.end() && !p; ++it) {
            p = it->Reserve(n);
        }

        if (p) {
            return p;
        }

        auto& block = memoryBlocks_.emplace_back(N);
        return block.Reserve(n);
    }

    void deallocate(T* p, std::size_t n) {
        auto it = std::find_if(memoryBlocks_.begin(), memoryBlocks_.end(), [p](const auto& m) { return m.Owns(p); });
        it->Free(p, n);

        if (it->Empty()) {
            memoryBlocks_.erase(it);
        }
    }

    template <typename U, typename... Args>
    void construct(U* p, Args&& ...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    template <typename U>
    void destroy(U* p) {
        p->~U();
    }

private:
    std::list<MemoryBlock<T>> memoryBlocks_;
};
