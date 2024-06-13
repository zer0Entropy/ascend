#include <filesystem>
#include <fstream>
#include "../include/resource/textFile.hpp"

TextFile::TextFile(const ResourceID& resourceID, std::string_view resourcePath):
    Resource{resourceID, Resource::TypeID::TextFile, resourcePath} {
    // If a file already exists at the specified path, load it
    if(std::filesystem::exists(std::filesystem::path{resourcePath})) {
        Reload();
    }
    // Otherwise, create an empty file with that filename
    else {
        std::ofstream output{this->GetPath(), std::ios_base::out};
        if(output.is_open()) {
            output.close();
        }
    }
}

void TextFile::Reload() {
    contents.clear();
    std::ifstream input{this->GetPath(), std::ios_base::in};
    if(input.good()) {
        while(!input.eof()) {
            std::string line{""};
            std::getline(input, line);
            contents.push_back(line);
        }
    }
    if(input.is_open()) {
        input.close();
    }
}

void TextFile::WriteToFilesystem() {
    std::ofstream output{this->GetPath(), std::ios_base::out};
    if(output.good()) {
        for(auto& line : contents) {
            output << line << "\n";
        }
    }
    if(output.is_open()) {
        output.close();
    }
}

void TextFile::Append(std::string_view text) {
    contents.push_back(std::string{text});
    std::ofstream output{this->GetPath(), std::ios_base::app};
    if(output.good()) {
        output << text << "\n";
    }
    if(output.is_open()) {
        output.close();
    }
}