#pragma once

#include <SFML/Graphics/Font.hpp>
#include "resource.hpp"

class Font: public Resource {
public:
    Font(const ResourceID& resID, std::string_view resPath);

    sf::Font&                   GetFont() const;
private:
    sf::Font                    font;
};