#pragma once

#include <string>
#include <boost/hana.hpp>

#include "calc/buttons.hpp"

namespace calc::buttons::specs::detail {

    constexpr auto is_store = [](auto buttonv) {
        return std::holds_alternative<store>(buttonv);
    };

    constexpr auto hold_store = [](auto state, auto self) {
        auto store_it = std::find_if(
            std::begin(state.buttons), std::end(state.buttons),
            is_store
        );

        auto store_idx = std::distance(std::begin(state.buttons), store_it);
        state.buttons = state.buttons.update(
            store_idx, [val = state.total](auto buttonv) {
                auto button = std::get<store>(buttonv);
                button.stored = val;
                return button;
            }
        );

        // if (button.stored && *button.stored == state.total)
        state.move_count += 1;

        return state;
    };

}
