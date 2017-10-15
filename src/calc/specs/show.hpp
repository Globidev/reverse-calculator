#pragma once

#include <string>

#include "calc/buttons.hpp"
#include "calc/specs/detail/spec.hpp"
#include "calc/specs/detail/show.hpp"

namespace calc::buttons::specs {

    namespace hana = boost::hana;

    using hana::_;
    using detail::spec;
    using namespace std::literals;

    // Concept map for stringifying buttons
    // All buttons MUST implement it {*1}
    const auto show_spec_map = hana::make_map(
        // rhs bound buttons will be passed their stringified rhs as parameter
        // {*2}
        spec           <add> = "+"s + _,
        spec           <sub> = "-"s + _,
        spec           <mul> = "x"s + _,
        spec           <div> = "/"s + _,
        spec         <power> = "^"s + _,
        spec        <append> = ":"s + _,
        spec      <meta_add> = "[+]"s + _,
        // stateful buttons will be passed their own instance as parameter
        // {*3}
        spec       <replace> = detail::show_replace,
        spec         <store> = detail::show_store,
        // other buttons return a string directly
        // {*4}
        spec   <rotate_left> = "shift <",
        spec  <rotate_right> = "shift >",
        spec    <erase_last> = "<<",
        spec      <opposite> = "+/-",
        spec       <reverse> = "reverse",
        spec           <sum> = "sum",
        spec        <mirror> = "mirror",
        spec         <inv10> = "inv10"
    );

    constexpr auto show = [](auto button) -> std::string {
        using namespace traits;
        constexpr auto t_button = hana::decltype_(button);

        // {*1}
        static_assert(
            t_button ^hana::in^ show_spec_map,
            "Missing show spec for button"
        );

        auto show_spec = show_spec_map[t_button];

        // {*2}
        if constexpr (is_rhs_bound(t_button))
            return show_spec(std::to_string(button.rhs));
        // {*3}
        else if constexpr (is_stateful(t_button))
            return show_spec(button);
        // {*4}
        else
            return show_spec;
    };

}
