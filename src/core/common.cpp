#include "../include/core/common.hpp"

sf::Color GetColorFromHex(std::string_view hex) {
    std::string hexRed{hex.substr(1, 2)};
    std::string hexGreen{hex.substr(3, 2)};
    std::string hexBlue{hex.substr(5, 2)};
    std::stringstream stream;
    int red{0};
    stream << hexRed;
    stream >> std::hex >> red;
    stream.clear();
    int green{0};
    stream << hexGreen;
    stream >> std::hex >> green;
    stream.clear();
    int blue{0};
    stream << hexBlue;
    stream >> std::hex >> blue;
    return sf::Color{   (sf::Uint8)red,
                        (sf::Uint8)green,
                        (sf::Uint8)blue,
                        (sf::Uint8)255};
}

std::string GetHexFromColor(const sf::Color& color) {
    std::stringstream stream;
    stream << std::hex << color.r;
    std::string hexRed{
        stream.str()
    };
    stream.clear();
    stream << std::hex << color.g;
    std::string hexGreen{
        stream.str()
    };
    stream.clear();
    stream << std::hex << color.b;
    std::string hexBlue{
        stream.str()
    };
    return "#" + hexRed + hexGreen + hexBlue;
}
