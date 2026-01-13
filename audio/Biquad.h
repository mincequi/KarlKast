#pragma once

#include <cstdint>
#include <span>
#include <vector>

#include "AudioFilterParams.h"

class Biquad {
public:
    enum class Type {
        Invalid     = 0,
        Peak        = 1,
        LowPass,
        HighPass,
        LowShelf,
        HighShelf,
        AllPass,
    };

    struct Coeffs {
        float b0 = 0.0, b1 = 0.0, b2 = 0.0;
        float a1 = 0.0, a2 = 0.0;
    };

    Biquad();

    /**
     * @brief Set number of cascades/passes.
     *
     * A biquad can have multiple cascades/passes to be applied to sample data.
     *
     * @param count
     */
    void setCascadeCount(uint8_t count);

    /**
     * @brief Set the sample rate.
     *
     * The sample rate is required to calculate the filter coefficients.
     *
     * @param rate
     */
    void setSampleRate(uint32_t rate);

    /**
     * @brief Get the filter parameters.
     *
     * @return FilterParams
     */
    AudioFilterParams filterParams() const;

    /**
     * @brief Set the filter parameters.
     *
     * The filter parameters are used to calculate the filter coefficients.
     *
     * @param type
     */
    void setFilterParams(const AudioFilterParams& type);

    void process(std::vector<float>& data);

    /**
     * @brief Check if the biquad is valid.
     *
     * @return true if valid, false otherwise
     */
    bool isValid() const;

    /**
     * @brief Get the coeffs of the biquad.
     *
     * @return Coeffs
     */
    inline const Coeffs& coeffs() const { return _coeffs; }

private:
    bool updateCoeffs();

    // The properties of a biquad
    uint32_t _sampleRate = 0;
    AudioFilterParams _filterParams;

    Coeffs _coeffs;

    // We have a history for each channel and cascade
    struct History {
        float x1 = 0.0, x2 = 0.0;
        float y1 = 0.0, y2 = 0.0;
    };
    std::vector<History> _history;
};
