#pragma once

#include <queue>

#include "calc/calculator.hpp"
#include "calc/specs.hpp"

#include "calc/solver/fwd.hpp"
#include "calc/solver/detail/bfs.hpp"

namespace calc::solver {

    auto bfs(calculator init_state, number_t goal) -> std::optional<actions_t> {
        detail::open_set_t open_set;
        open_set.push({ init_state });

        while (!open_set.empty()) {
            auto current = open_set.top();

            if (current.state.total == goal)
                return current.actions;

            open_set.pop();

            for (auto buttonv: current.state.buttons) {
                std::visit(
                    [&](auto button) {
                        detail::queue_reachable_paths(current, open_set, button);
                    },
                    buttonv
                );
            }
        }

        return std::nullopt;
    }

}
