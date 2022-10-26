#pragma once

#include <cstdlib>
#include <memory>

template <typename T, class Allocator = std::allocator<T> >
class List {
private:
    struct Node {
        Node(const T& val) : value(val), pNext(nullptr) {}
        T value;
        Node* pNext;
    };

    using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    Node* getTail(Node* pNode) {
        return pNode->pNext ? getTail(pNode->pNext) : pNode;
    }

    void clear(Node* pNode) {
        if (pNode->pNext) {
            clear(pNode->pNext);
            pNode->pNext = nullptr;
        }

        std::allocator_traits<NodeAllocator>::destroy(allocator_, pNode);
        std::allocator_traits<NodeAllocator>::deallocate(allocator_, pNode, 1);
    }

    Node* pHead_;
    NodeAllocator allocator_;

public:
    List() : pHead_(nullptr) {}
    ~List() { clear(); }

    void push_back(const T& value) {
        Node* pNode = std::allocator_traits<NodeAllocator>::allocate(allocator_, 1);
        std::allocator_traits<NodeAllocator>::construct(allocator_, pNode, value);

        if (!pHead_) {
            pHead_ = pNode;
        } else {
            getTail(pHead_)->pNext = pNode;
        }
    }

    void clear() {
        if (!pHead_) {
            return;
        }

        clear(pHead_);
        pHead_ = nullptr;
    }

    class iterator {
        public:
            iterator(Node* pNode) : pNode_(pNode) {}
            T& operator*() {
                return pNode_->value;
            }

            iterator& operator++() {
                pNode_ = pNode_->pNext;
                return *this;
            }

            bool operator==(const iterator& other) const {
                return pNode_ == other.pNode_;
            }

            bool operator!=(const iterator& other) const {
                return pNode_ != other.pNode_;
            }

        private:
            Node* pNode_;
    };

    iterator begin() const {
        return iterator(pHead_);
    }

    iterator end() const {
        return iterator(nullptr);
    }
};
