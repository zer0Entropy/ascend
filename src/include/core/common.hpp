#pragma once

#include <array>
#include <string>
#include <sstream>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>   
#include <SFML/System/Vector2.hpp>

enum class Alignment{
    Left,
    Center,
    Right
};
constexpr int NumAlignments = 3;
constexpr std::array<std::string, NumAlignments> AlignmentNames{
    "left", "center", "right"
};

enum class Orientation {
    Horizontal,
    Vertical
};
constexpr int NumOrientations = 2;
constexpr std::array<std::string, NumOrientations> OrientationNames{
    "horizontal", "vertical"
};

template<typename T>
bool IsInBounds(sf::Vector2<T> position, sf::Rect<T> rect) {
    bool inBounds{false};
    if(     position.x >= rect.left
        &&  position.x < rect.left + rect.width
        &&  position.y >= rect.top
        &&  position.y < rect.top + rect.height) {
        inBounds = true;
    }
    return inBounds;
}

sf::Color GetColorFromHex(std::string_view hex);
std::string GetHexFromColor(const sf::Color& color);
