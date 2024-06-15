#pragma once

#include <string>
#include <vector>
#include "../core/entity.hpp"

struct MenuOption {
    Entity                  widget;
    std::string             name;

    MenuOption():
        widget{0},
        name{""} {}
    MenuOption(Entity widget, std::string nameStr):
        widget{widget},
        name{nameStr} {}
    MenuOption(const MenuOption& copy) = default;
    MenuOption(MenuOption&& move) = default;
    ~MenuOption() = default;
    MenuOption& operator=(const MenuOption& copy) = default;
    MenuOption& operator=(MenuOption&& move) = default;
};

struct Menu {
    std::vector<MenuOption> options;
    MenuOption*             selectedOption;

    Menu():
        selectedOption{nullptr} {}
    Menu(const std::vector<MenuOption>& copyOptions, MenuOption* selected = nullptr):
        options{copyOptions},
        selectedOption{selected} {}
    Menu(const Menu& copy) = default;
    Menu(Menu&& move) = default;
    ~Menu() = default;
    Menu& operator=(const Menu& copy) = default;
    Menu& operator=(Menu&& move) = default;
};