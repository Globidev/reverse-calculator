#pragma once

#include "calc/buttons.hpp"

#include "calc/specs/detail/spec.hpp"
#include "calc/specs/detail/parse.hpp"

namespace calc::buttons::specs {

    namespace hana = boost::hana;
    namespace qi = boost::spirit::qi;

    using detail::spec;

    // Concept map for parsing buttons
    // All buttons MUST implement it {*1}
    template <class It, class Skipper>
    const auto parse_spec_map = detail::make_parse_spec_map<It, Skipper>(
        // rhs bound buttons will be prefix parsed followed by a number_t parser
        // {*d2}
        spec          <add> = "+",
        spec          <sub> = "-",
        spec          <mul> = "x",
        spec          <div> = "/",
        spec        <power> = "^",
        spec       <append> = ":",
        spec     <meta_add> = "[+]",
        // stateful buttons will be parsed using a custom given parser rule
        // {*d3}
        spec      <replace> = detail::parse_replace<It, Skipper>,
        spec        <store> = detail::parse_store<It, Skipper>,
        // non rhs bound / non stateful buttons will be parsed literally
        // {*d4}
        spec  <rotate_left> = "shift<",
        spec <rotate_right> = "shift>",
        spec   <erase_last> = "<<",
        spec     <opposite> = "+/-",
        spec      <reverse> = "reverse",
        spec          <sum> = "sum",
        spec       <mirror> = "mirror",
        spec        <inv10> = "inv10"
    );

    template <class It, class... Ts>
    constexpr auto make_grammar(hana::basic_type<It>, hana::basic_type<std::variant<Ts...>>) {
        using skipper_t = qi::blank_type;
        using synthetized_rule_t = qi::rule<It, std::variant<Ts...>(), skipper_t>;

        // {*1}
        static_assert(
            hana::all_of(
                hana::make_tuple(hana::type_c<Ts>...),
                [](auto type) {
                    return type ^hana::in^ parse_spec_map<It, skipper_t>;
                }
            ),
            "Missing parser spec for button"
        );

        // Variadic alternative of all the button rules
        return synthetized_rule_t {
            ( parse_spec_map<It, skipper_t>[hana::type_c<Ts>]
            | ... )
        };
    }

    constexpr auto parse = [](auto input) -> std::optional<button_v> {
        static const auto grammar = make_grammar(
            hana::decltype_(std::begin(input)),
            hana::type_c<button_v>
        );

        button_v buttonv;

        auto begin_it = std::begin(input),
             end_it   = std::end(input);

        auto parsed = qi::phrase_parse(
            begin_it, end_it,
            grammar,
            qi::blank,
            buttonv
        );

        if (parsed && begin_it == end_it)
            return buttonv;
        else
            return std::nullopt;
    };

}
