#pragma once

#include <string>
#include <vector>
#include "../core/entity.hpp"

struct MenuOption {
    Entity                  widgetID;
    std::string             name;
};

struct Menu {
    std::vector<MenuOption> options;
    MenuOption*             selectedOption;
};