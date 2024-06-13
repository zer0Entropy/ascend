#pragma once

#include <vector>
#include "resource.hpp"

class TextFile: public Resource {
public:
    TextFile(const ResourceID& resourceID, std::string_view resourcePath);
    
    void                                Reload();
    void                                WriteToFilesystem();
    void                                Append(std::string_view text);
private:
    std::vector<std::string>            contents;
};