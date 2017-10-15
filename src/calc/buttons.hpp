#pragma once

#include <variant>
#include <immer/vector.hpp>
#include <optional>
#include <string>

#include <boost/hana.hpp>

#include "calc/fwd.hpp"

namespace calc::buttons {
    namespace hana = boost::hana;

    namespace traits {
        struct rhs_bound { number_t rhs; };
        struct stateful { };
        struct mutable_rhs { };
        struct holdable { };

        template <class Tag>
        constexpr auto has_trait = hana::partial(
            hana::trait<std::is_base_of>,
            hana::type_c<Tag>
        );

        constexpr auto is_rhs_bound    = has_trait<rhs_bound>;
        constexpr auto is_stateful     = has_trait<stateful>;
        constexpr auto has_mutable_rhs = has_trait<mutable_rhs>;
        constexpr auto is_holdable     = has_trait<holdable>;
    }

    struct add:      traits::rhs_bound, traits::mutable_rhs { };
    struct sub:      traits::rhs_bound, traits::mutable_rhs { };
    struct mul:      traits::rhs_bound, traits::mutable_rhs { };
    struct div:      traits::rhs_bound, traits::mutable_rhs { };
    struct power:    traits::rhs_bound, traits::mutable_rhs { };
    struct append:   traits::rhs_bound, traits::mutable_rhs { };
    struct meta_add: traits::rhs_bound                      { };

    struct rotate_left  { };
    struct rotate_right { };
    struct erase_last   { };
    struct opposite     { };
    struct reverse      { };
    struct sum          { };
    struct mirror       { };
    struct inv10        { };

    struct replace: traits::stateful {
        replace() = default;
        replace(std::string from, std::string to): from(from), to(to) { }

        std::string from, to;
    };

    struct store: traits::stateful, traits::holdable {
        std::optional<number_t> stored;
    };

    using button_v = std::variant<
        add, sub, mul, div,
        power, append, meta_add,
        rotate_left, rotate_right,
        erase_last, opposite,
        reverse, sum,
        mirror, inv10,
        replace, store
    >;

    using buttons_t = immer::vector<button_v>;
}
