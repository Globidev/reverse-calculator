#pragma once

#include <boost/hana.hpp>

#include "calc/buttons.hpp"
#include "calc/specs/detail/spec.hpp"
#include "calc/specs/detail/operations.hpp"

namespace calc::buttons::specs {

    namespace hana = boost::hana;

    using hana::_;
    using detail::spec;

    // Concept map for operating on numbers
    // All buttons MUST implement it {*1}
    constexpr auto operator_spec_map = hana::make_map(
        // rhs bound buttons will be passed the number and their rhs as parameters
        // {*2}
        spec          <add> = _ + _,
        spec          <sub> = _ - _,
        spec          <mul> = _ * _,
        spec          <div> = detail::div,
        spec        <power> = detail::pow,
        spec       <append> = detail::append,
        spec     <meta_add> = hana::arg<1>,
        // stateful buttons will be passed the number and their own instance as parameters
        // {*3}
        spec      <replace> = detail::replace,
        spec        <store> = detail::store,
        // other buttons will only be passed the number to operate on
        // {*4}
        spec  <rotate_left> = detail::rotate_left,
        spec <rotate_right> = detail::rotate_right,
        spec   <erase_last> = _ / 10,
        spec     <opposite> = - _,
        spec      <reverse> = detail::reverse,
        spec          <sum> = detail::sum,
        spec       <mirror> = detail::mirror,
        spec        <inv10> = detail::inv10
    );

    constexpr auto mutate_number = [](number_t lhs, auto button) -> number_t {
        using namespace traits;
        constexpr auto t_button = hana::decltype_(button);

        // {*1}
        static_assert(
            t_button ^hana::in^ operator_spec_map,
            "Missing operator spec for button"
        );

        constexpr auto op_spec = operator_spec_map[t_button];

        // {*2}
        if constexpr (is_rhs_bound(t_button))
            return op_spec(lhs, button.rhs);
        // {*3}
        else if constexpr (is_stateful(t_button))
            return op_spec(lhs, button);
        // {*4}
        else
            return op_spec(lhs);
    };

    // Concept map for operating on the list of buttons
    // Implementation is optional
    // Buttons who choose to not implement it will simply let the list intact
    // {*5}
    constexpr auto buttons_modifier_spec_map = hana::make_map(
        // Implementers will be passed the list of buttons and their own instance as parameters
        // {*6}
        spec <meta_add> = detail::meta_add
    );

    constexpr auto mutate_buttons = [](const buttons_t & buttons, auto button) -> buttons_t {
        constexpr auto t_button = hana::decltype_(button);

        // {*6}
        if constexpr (t_button ^hana::in^ buttons_modifier_spec_map)
            return buttons_modifier_spec_map[t_button](buttons, button);
        // {*5}
        else
            return buttons;
    };

    constexpr auto press = [](const calculator & state, auto button) -> calculator {
        return {
            mutate_buttons(state.buttons, button),
            mutate_number(state.total, button),
            state.move_count + 1
        };
    };

}
