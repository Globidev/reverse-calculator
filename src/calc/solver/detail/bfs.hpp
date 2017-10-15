#pragma once

#include <boost/hana.hpp>

#include "calc/calculator.hpp"

namespace calc::solver::detail {

    namespace hana = boost::hana;

    struct node {
        calculator state;
        actions_t actions = { };
    };

    constexpr auto operator<(const node & n1, const node & n2) {
        return n1.state.move_count > n2.state.move_count;
    }

    using open_set_t = std::priority_queue<node, std::deque<node>>;

    template <action::type_e action_type>
    constexpr auto action_type_c = hana::int_c<static_cast<int>(action_type)>;

    using buttons::specs::press;
    using buttons::specs::hold;

    constexpr auto action_map = hana::make_map(
        hana::make_pair(action_type_c<action::type_e::Press>, press),
        hana::make_pair(action_type_c<action::type_e::Hold>, hold)
    );

    template <action::type_e action_type>
    constexpr auto action_path = [](const node & n, auto button) -> node {
        constexpr auto act = action_map[action_type_c<action_type>];

        auto new_action = action { action_type, button };
        auto new_actions = n.actions.push_back(new_action);

        return { act(n.state, button), new_actions };
    };

    constexpr auto queue_reachable_paths = [](const node & node, open_set_t & set, auto button) {
        constexpr auto t_button = hana::decltype_(button);
        constexpr auto is_holdable = buttons::traits::is_holdable(t_button);

        set.push(action_path<action::type_e::Press>(node, button));
        if constexpr (is_holdable)
            set.push(action_path<action::type_e::Hold>(node, button));
    };
}
