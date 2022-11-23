#pragma once

#include <memory>
#include <iostream>
#include "model.h"
#include "view.h"

class Controller {
public:
    Controller()
        : pModel_(nullptr)
        , pView_(nullptr)
    {}

    ~Controller() = default;

    void NewDocument() {
        pModel_ = std::make_unique<Model::Document>();
        pView_ = std::make_unique<View::Document>(*pModel_);
    }

    void AddSquare(const unsigned int side) {
        pModel_->AddObject<Model::Square>(side);
    }

    void AddCircle(const unsigned int radius) {
        pModel_->AddObject<Model::Circle>(radius);
    }

    void DeleteLastObject() {
        pModel_->RmLastObject();
    }

    void ExportDocument(const std::string& file) const {
        const std::string documentContent = pModel_->Serialize();

        std::cout << "Exporting document content '" << documentContent << "' into file '" << file << "'" << std::endl;
    }

    void ImportDocument(const std::string& file) {
        std::cout << "Importing document content from file '" << file << "'" << std::endl;

        // TODO: read file content
        const std::string fileContent = "";

        pModel_ = std::unique_ptr<Model::Document>(Model::Document::Deserialize(fileContent));
        pView_ = std::make_unique<View::Document>(*pModel_);
    }

private:
    std::unique_ptr<Model::Document> pModel_;
    std::unique_ptr<View::Document> pView_;
};
