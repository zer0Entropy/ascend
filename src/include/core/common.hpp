#pragma once

#include <array>
#include <string>

struct Point {
    unsigned int        x;
    unsigned int        y;
};

enum class Alignment{
    Left,
    Center,
    Right
};
constexpr int NumAlignments = 3;
constexpr std::array<std::string, NumAlignments> AlignmentNames{
    "left", "center", "right"
};