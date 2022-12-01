#pragma once

#include <vector>
#include <stdexcept>

namespace Model {

class Square;
class Circle;

///
class IObserver {
public:
    virtual ~IObserver() = default;

    virtual void OnNewObject(const Square&) = 0;
    virtual void OnNewObject(const Circle&) = 0;

    virtual void OnRmLastObject() = 0;
};

///
class Observable {
public:
    void AddObserver(IObserver* observer) {
        observers_.push_back(observer);
    }

    void NotifyRmLastObject() {
        for (auto observer : observers_) {
            observer->OnRmLastObject();
        }
    }

    template <typename T>
    void NotifyNewObject(const T& obj) {
        for (auto observer : observers_) {
            observer->OnNewObject(obj);
        }
    }

private:
    std::vector<IObserver*> observers_;
};

}
