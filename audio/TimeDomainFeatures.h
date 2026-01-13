#pragma once

struct TimeDomainFeatures {
    int peak = -100;
    int rms = -100;
    double ste = 0.0f;
    double zcr = 0;
    int frameSize = 0;

    TimeDomainFeatures round() const {
        return TimeDomainFeatures{
            .peak = peak,
            .rms = rms,
            .ste = std::round(ste),
            .zcr = std::round(zcr),
            .frameSize = frameSize
        };
    }
};
