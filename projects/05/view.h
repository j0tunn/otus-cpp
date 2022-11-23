#pragma once

#include "observer.h"
#include "model.h"
#include <list>
#include <memory>
#include <iostream>
#include <algorithm>

namespace View {

///
class IDrawable {
public:
    virtual ~IDrawable() = default;

    virtual void Draw() const = 0;
};

///
class Square : public IDrawable {
public:
    explicit Square(const Model::Square& model) : model_(model) {}

    void Draw() const override {
        std::cout << "square with side " << std::to_string(model_.GetSide()) << std::endl;
    }

private:
    const Model::Square& model_;
};

///
class Circle : public IDrawable {
public:
    explicit Circle(const Model::Circle& model) : model_(model) {}

    void Draw() const override {
        std::cout << "circle with radius " << std::to_string(model_.GetRadius()) << std::endl;
    }

private:
    const Model::Circle& model_;
};

///
class Document : public Model::IObserver {
public:
    Document(Model::Document& model) {
        model.AddObserver(this);
    }

    void OnRmLastObject() override {
        subViews_.pop_back();
        Draw();
    }

    void OnNewObject(const Model::Square& squareModel) override {
        AddSubView_<View::Square>(squareModel);
    }

    void OnNewObject(const Model::Circle& circleModel) override {
        AddSubView_<View::Circle>(circleModel);
    }

private:
    template <typename T, typename U>
    void AddSubView_(const U& model) {
        subViews_.push_back(std::unique_ptr<View::IDrawable>(new T(model)));
        Draw();
    }

    void Draw() const {
        std::cout << "--->" << std::endl;
        std::for_each(subViews_.begin(), subViews_.end(), [](const auto& view) { view->Draw(); });
        std::cout << "<---" << std::endl << std::endl;
    }

    std::list<std::unique_ptr<View::IDrawable> > subViews_;
};

}
