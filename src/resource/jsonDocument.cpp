#include <fstream>
#include "../include/resource/jsonDocument.hpp"

JSONDocument::JSONDocument(const ResourceID& resID, std::string_view resPath):
    Resource{resID, Resource::TypeID::JSONDocument, resPath} {
    std::ifstream       fileInput{this->GetPath(), std::ios_base::in};
    if(fileInput.is_open() && fileInput.good()) {
        fileInput >> json;
        fileInput.close();
    }
}

const nlohmann::json& JSONDocument::GetJSON() const {
    return json;
}

void JSONDocument::SetJSON(const nlohmann::json& copyJSON) {
    json = copyJSON;
    std::ofstream       fileOutput{this->GetPath(), std::ios_base::out};
    if(fileOutput.is_open()) {
        while(fileOutput.good()) {
            fileOutput << json;
        }
        fileOutput.close();
    }
}