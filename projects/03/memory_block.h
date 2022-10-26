#pragma once

#include <memory>
#include <map>
#include "memory_mgr.h"

template <typename T>
class MemoryBlock {
public:
    MemoryBlock(size_t size)
        : size_(size)
        , p_(reinterpret_cast<T*>(MemoryMgr::allocate(size * sizeof(T))), &MemoryMgr::deallocate)
        , freeChunks_{{size, 0}}
    {}

    T* Reserve(size_t size) {
        auto it = freeChunks_.lower_bound(size);
        if (it == std::end(freeChunks_)) {
            return nullptr;
        }

        const size_t chunkSize = it->first;
        const size_t chunkOffset = it->second;

        freeChunks_.erase(it);

        if (chunkSize > size) {
            freeChunks_.insert({chunkSize - size, chunkOffset + size});
        }

        return p_.get() + chunkOffset;
    }

    void Free(T* p, size_t size) {
        const size_t offset = p - p_.get();

        auto it = std::find_if(freeChunks_.begin(), freeChunks_.end(),
            [offset, size](const auto& chunk) { return offset + size == chunk.second || chunk.second + chunk.first == offset; });

        if (it != freeChunks_.end()) {
            freeChunks_.insert({it->first + size, std::min(it->second, offset)});
            freeChunks_.erase(it);
        } else {
            freeChunks_.insert({size, offset});
        }
    }

    bool Owns(T* p) const {
        return p_.get() <= p && p <= p_.get() + size_;
    }

    bool Empty() const {
        return freeChunks_.find(size_) != std::end(freeChunks_);
    }

private:
    size_t size_;
    std::unique_ptr<T, decltype(&MemoryMgr::deallocate)> p_;
    std::multimap<size_t, size_t> freeChunks_;
};
