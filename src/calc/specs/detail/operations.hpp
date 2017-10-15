#pragma once

#include <cmath>
#include <numeric>

#include <immer/vector.hpp>
#include <immer/vector_transient.hpp>

#include "calc/fwd.hpp"

namespace calc::buttons::specs::detail {

    constexpr auto string_transform = [](auto f) {
        return [=](auto lhs, auto... ctx) {
            auto transformed = f(std::to_string(std::abs(lhs)), ctx...);

            number_t ret;
            if constexpr (std::is_same_v<number_t, decltype(transformed)>)
                ret = transformed;
            else {
                try {
                    ret = std::stoi(transformed);
                }
                catch (std::out_of_range) {
                    return lhs;
                }
            }

            return lhs >= 0 ? ret : -ret;
        };
    };

    constexpr auto div = [](auto lhs, auto rhs) {
        if (lhs % rhs == 0)
            return lhs / rhs;
        else
            return lhs;
    };

    constexpr auto pow = [](auto lhs, auto rhs) {
        return std::pow(lhs, rhs);
    };

    constexpr auto append = [](auto lhs, auto rhs) {
        // TODO: optimize ?
        auto power = 10;
        while (power <= rhs) power *= 10;

        return lhs * power + rhs;
    };

    constexpr auto rotate_left = string_transform([](auto s_lhs) {
        std::rotate(
            std::begin(s_lhs), std::begin(s_lhs) + 1,
            std::end(s_lhs)
        );
        return s_lhs;
    });

    constexpr auto rotate_right = string_transform([](auto s_lhs) {
        std::rotate(
            std::rbegin(s_lhs), std::rbegin(s_lhs) + 1,
            std::rend(s_lhs)
        );
        return s_lhs;
    });

    constexpr auto reverse = string_transform([](auto s_lhs) {
        std::reverse(std::begin(s_lhs), std::end(s_lhs));

        return s_lhs;
    });

    constexpr auto sum = string_transform([](auto s_lhs) {
        return std::accumulate(
            std::begin(s_lhs), std::end(s_lhs),
            number_t { 0 }, [](auto acc, char c) { return acc + c - '0'; }
        );
    });

    constexpr auto mirror = string_transform([](auto s_lhs) {
        auto reversed = s_lhs;
        std::reverse(std::begin(reversed), std::end(reversed));

        return s_lhs + reversed;
    });

    constexpr auto inv10 = string_transform([](auto s_lhs) {
        return std::accumulate(
            std::begin(s_lhs), std::end(s_lhs),
            number_t { 0 }, [](auto acc, char c) {
                auto n = c - '0';
                auto inv = (n == 0 ? 0 : 10 - n);

                return acc * 10 + inv;
            }
        );
    });

    constexpr auto replace = string_transform([](auto s_lhs, auto self) {
        std::string::size_type idx = 0;
        while ((idx = s_lhs.find(self.from, idx)) != std::string::npos)
        {
            s_lhs.replace(idx, self.from.size(), self.to);
            idx += self.from.size();
        }
        return s_lhs;
    });

    constexpr auto store = [](auto lhs, auto self) {
        if (self.stored)
            return append(lhs, *self.stored);
        else
            return lhs;
    };

    constexpr auto increment_rhs = [](auto n, auto button) -> button_v {
        if constexpr (traits::has_mutable_rhs(hana::decltype_(button))) {
            button.rhs += n;
            return button;
        }
        else
            return button;
    };

    constexpr auto meta_add = [](auto buttons, auto self) {
        buttons_t modified;
        auto transient_modified = modified.transient();

        std::transform(
            std::begin(buttons), std::end(buttons),
            std::back_inserter(transient_modified),
            [=](auto buttonv) {
                return std::visit(
                    [rhs = self.rhs](auto button) {
                        return increment_rhs(rhs, button);
                    },
                    buttonv
                );
            }
        );

        return transient_modified.persistent();
    };
}
