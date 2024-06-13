#include "../include/resource/font.hpp"

Font::Font(const ResourceID& resID, std::string_view resPath):
    Resource{resID, Resource::TypeID::Font, resPath} {
    font.loadFromFile(this->GetPath());
}

sf::Font& Font::GetFont() const {
    return const_cast<sf::Font&>(font);
}