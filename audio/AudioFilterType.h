#pragma once

#include <cstdint>

enum class AudioFilterType : int8_t {
    Bypass  = 0,

    Gain,

    // Linear filters
    Peaking,
    LowPass,
    HighPass,
    LowShelf,
    HighShelf,

    AllPass,
};
