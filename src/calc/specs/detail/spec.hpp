#pragma once

#include <boost/hana.hpp>

namespace calc::buttons::specs::detail {

    template <class T>
    struct assignable_spec {

        template <class U>
        constexpr auto operator=(U v) const {
            return boost::hana::make_pair(boost::hana::type_c<T>, v);
        }

    };

    template <class T>
    constexpr auto spec = assignable_spec<T> { };

}
