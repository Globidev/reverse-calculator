#pragma once

#include <string>
#include <boost/hana.hpp>

namespace calc::buttons::specs::detail {

    constexpr auto show_replace = [](auto self) {
        return self.from + "=>" + self.to;
    };

    constexpr auto show_store = [](auto self) {
        using namespace std::literals;

        auto s = "store"s;
        if (self.stored)
            s += " (" + std::to_string(*self.stored) + ")";
        return s;
    };

}
