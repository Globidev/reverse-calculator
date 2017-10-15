#pragma once

#include <boost/hana.hpp>

#include "calc/calculator.hpp"
#include "calc/specs/detail/spec.hpp"
#include "calc/specs/detail/hold.hpp"

namespace calc::buttons::specs {

    namespace hana = boost::hana;

    using detail::spec;

    // Concept map for describing the hold logic
    // Implementation is optional
    // Buttons who choose to not implement it will simply let the state intact
    // {*1}
    constexpr auto hold_spec_map = hana::make_map(
        // Implementers will be passed the state their own instance as parameters
        // {*2}
        spec <store> = detail::hold_store
    );

    constexpr auto hold = [](const calculator & state, auto button) -> calculator {
        constexpr auto t_button = hana::decltype_(button);

        // {*2}
        if constexpr (t_button ^hana::in^ hold_spec_map)
            return hold_spec_map[t_button](state, button);
        // {*1}
        else
            return state;
    };

}
