#pragma once

#include <map>
#include <array>
#include <type_traits>
#include <utility>

template <typename ElementAccessor, unsigned int MAX_DIMENSION, unsigned int N>
class DimensionAccessor {
public:
    using Coordinates = std::array<unsigned int, MAX_DIMENSION - N>;
    using NextAccessor = typename std::conditional<N == 1, ElementAccessor, DimensionAccessor<ElementAccessor, MAX_DIMENSION, N - 1>>::type;

    DimensionAccessor(typename ElementAccessor::Storage& storage, Coordinates&& coords)
        : storage_(storage)
        , coords_(coords)
    {}

    NextAccessor operator[](unsigned int i) {
        std::array<unsigned int, MAX_DIMENSION - N + 1> arr;
        std::copy(coords_.begin(), coords_.end(), arr.begin());
        arr[arr.size() - 1] = i;

        return NextAccessor(storage_, std::move(arr));
    }

private:
    typename ElementAccessor::Storage& storage_;
    Coordinates coords_;
};

template <typename T, typename T::mapped_type DEFAULT_VALUE>
class ElementProxy {
public:
    using Storage = T;
    using Val = typename Storage::mapped_type;
    using Key = typename Storage::key_type;

    ElementProxy(Storage& storage, Key&& key)
        : storage_(storage)
        , key_(key) {}

    operator Val() const {
        const auto it = storage_.find(key_);

        return it == storage_.end() ? DEFAULT_VALUE : it->second;
    }

    ElementProxy& operator=(const Val val) {
        if (val == DEFAULT_VALUE) {
            storage_.erase(key_);
        } else {
            storage_[key_] = val;
        }

        return *this;
    }

private:
    Storage& storage_;
    Key key_;
};

template <typename Storage>
class RestrictedIterator {
public:
    RestrictedIterator(typename Storage::iterator&& it) : it_(it) {}

    RestrictedIterator& operator++() {
        ++it_;
        return *this;
    }

    bool operator!=(const RestrictedIterator& other) const {
        return it_ != other.it_;
    };

    typename Storage::value_type operator*() {
        return *it_;
    }

private:
    typename Storage::iterator it_;
};

template <typename T, unsigned int N, T DEFAULT_VALUE>
class Matrix {
public:
    using Coordinates = std::array<unsigned int, N>;
    using Storage = std::map<Coordinates, T>;
    using Accessor = typename DimensionAccessor<ElementProxy<Storage, DEFAULT_VALUE>, N, N>::NextAccessor;
    using Iterator = RestrictedIterator<Storage>;

    unsigned int size() const {
        return storage_.size();
    }

    Accessor operator[](unsigned int x) {
        return Accessor(storage_, {x});
    }

    Iterator begin() {
        return Iterator(storage_.begin());
    }

    Iterator end() {
        return Iterator(storage_.end());
    }

private:
    Storage storage_;
};
