#pragma once

#include <boost/hana.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include "calc/buttons.hpp"

namespace calc::buttons::specs::detail {

    namespace hana = boost::hana;

    using boost::phoenix::bind;
    using boost::phoenix::construct;

    using namespace boost::spirit::qi;

    template <class T>
    constexpr auto aggregate_construct = [](auto... vals) {
        return T { vals... };
    };

    template <class Button, class It, class Skipper>
    constexpr auto parser_spec_impl = [](auto parser) {
        using namespace traits;
        using rule_t = rule<It, Button(), Skipper>;

        constexpr auto t_button = hana::type_c<Button>;
        rule_t rule;

        // {*d2}
        if constexpr (is_rhs_bound(t_button)) {
            auto build_button = bind(aggregate_construct<Button>, _1);
            rule = (lit(parser) >> int_parser<number_t>{})[_val = build_button];
        }
        // {*d3}
        else if constexpr (is_stateful(t_button))
            rule = parser;
        // {*d4}
        else
            rule = lit(parser)[construct<Button>];

        return hana::make_pair(t_button, rule);
    };

    template <class It, class Skipper>
    constexpr auto make_parse_spec_map = [](auto... specs) {
        return hana::make_map(
            parser_spec_impl<
                typename decltype(hana::decltype_(hana::first(specs)))::type,
                It,
                Skipper
            >(hana::second(specs))
            ...
        );
    };

    // Weird copy semantics of spirit forces us to explicit the rule type
    // which makes us to dependent on It and Skipper...

    template <class It, class Skipper>
    const rule<It, replace(), Skipper> parse_replace =
        (  as_string[+char_("0-9")]
        >> "=>"
        >> as_string[+char_("0-9")]
        )
        [_val = construct<replace>(_1, _2)]
    ;

    template <class It, class Skipper>
    const rule<It, store(), Skipper> parse_store =
        lit("store")[construct<store>]
    ;
}
