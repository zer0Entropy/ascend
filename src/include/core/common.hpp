#pragma once

#include <array>
#include <string>

enum class Alignment{
    Left,
    Center,
    Right
};
constexpr int NumAlignments = 3;
constexpr std::array<std::string, NumAlignments> AlignmentNames{
    "left", "center", "right"
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