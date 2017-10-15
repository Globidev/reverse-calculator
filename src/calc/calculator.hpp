#pragma once

#include "calc/buttons.hpp"

namespace calc {

    struct calculator {
        buttons::buttons_t buttons;

        number_t total = 0;
        unsigned move_count = 0;
    };

}
