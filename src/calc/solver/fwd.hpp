#pragma once

#include <iostream>
#include <immer/vector.hpp>

#include "calc/buttons.hpp"
#include "calc/specs.hpp"

namespace calc::solver {

    namespace hana = boost::hana;

    struct action {
        enum class type_e { Press, Hold };

        type_e type;
        buttons::button_v button;
    };

    std::ostream & operator<<(std::ostream & os, action action) {
        os << '(' << std::visit(buttons::specs::show, action.button);
        if (action.type == action::type_e::Hold)
            os << " (hold)";
        os << ')';
        return os;
    }

    using actions_t = immer::vector<action>;

    std::ostream & operator<<(std::ostream & os, actions_t actions) {
        std::copy(
            std::begin(actions), std::end(actions),
            std::experimental::make_ostream_joiner(os, " -> ")
        );
        return os;
    }
}
