#pragma once

#include <memory>
#include <string>
#include <list>
#include <numeric>
#include "observer.h"

namespace Model {

///
class IObject {
public:
    virtual ~IObject() = default;
    virtual std::string Serialize() const = 0;
};

///
class Square : public IObject {
public:
    Square(const unsigned int side) : side_(side) {}

    std::string Serialize() const override {
        return "square:" + std::to_string(side_);
    }

    unsigned int GetSide() const {
        return side_;
    }

private:
    unsigned int side_;
};

///
class Circle : public IObject {
public:
    Circle(const unsigned int radius) : radius_(radius) {}

    std::string Serialize() const override {
        return "circle:" + std::to_string(radius_);
    }

    unsigned int GetRadius() const {
        return radius_;
    }

private:
    unsigned int radius_;
};

///
class Document : public Observable {
public:
    static std::unique_ptr<Document> Deserialize(const std::string& str) {
        std::unique_ptr<Document> pDoc(new Document());

        // TODO: parse string, for each obj call pDoc->AddObject()

        return pDoc;
    }

    template <typename T, typename... Args>
    void AddObject(Args... args) {
        auto pObj = new T(args...);
        objects_.push_back(std::unique_ptr<IObject>(pObj));

        NotifyNewObject(*pObj);
    }

    void RmLastObject() {
        objects_.pop_back();

        NotifyRmLastObject();
    }

    std::string Serialize() const {
        return std::accumulate(objects_.begin(), objects_.end(), std::string(""), [](auto res, const auto& o) { return res + o->Serialize() + ";"; });
    }

private:
    std::list<std::unique_ptr<IObject> > objects_;
};

}
