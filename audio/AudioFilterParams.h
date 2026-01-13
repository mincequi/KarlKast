#pragma once

#include <optional>
#include <span>

#include "AudioFilterType.h"

struct AudioFilterParams {
    AudioFilterType type = AudioFilterType::Bypass;
    double f = 0.0;
    double g = 0;
    double q = 0.707;

    inline bool isValid() const {
        return (type != AudioFilterType::Bypass && f >= 20.0 && f <= 4000.0 && q > 0);
    }

    inline bool operator==(const AudioFilterParams& other) {
        return type == other.type &&
               f == other.f &&
               g == other.g &&
               q == other.q;
    }

    // assignment operator
    AudioFilterParams& operator=(const AudioFilterParams& other) {
        type = other.type;
        f = other.f;
        g = other.g;
        q = other.q;
        return *this;
    }
};
