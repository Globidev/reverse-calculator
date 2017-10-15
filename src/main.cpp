#include <iostream>
#include <experimental/iterator>

#include "calc/solver/bfs.hpp"

int main(int argc, const char *argv[]) {
    using namespace calc;
    using namespace buttons;

    if (argc < 4)
        return -1;

    auto start = std::stoi(argv[1]),
         goal = std::stoi(argv[2]);

    buttons_t buttons;
    std::for_each(argv + 3, argv + argc, [&](std::string arg) {
        auto mb_buttonv = specs::parse(arg);
        if (mb_buttonv)
            buttons = buttons.push_back(std::move(*mb_buttonv));
        else
            std::cerr << "Failed to parse: \"" << arg << "\"\n";
    });

    calculator state { buttons, start };

    auto steps = solver::bfs(state, goal);

    if (steps)
        std::cout << *steps;
    else
        std::cout << "impossible";
    std::cout << std::endl;
}
