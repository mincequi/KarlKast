#pragma once

#include <span>

#include "AudioFilterParams.h"
#include "Biquad.h"

/**
 * @brief A filter.
 *
 * A filter is a wrapper around one or more biquads.
 */
class AudioFilter {
public:
    AudioFilter();

    bool isValid() const;
    AudioFilterParams filterParams() const;
    void setFilterParams(const AudioFilterParams& params);
    //void setCascadeCount(uint8_t count);
    void setSampleRate(uint32_t rate);

    void process(std::vector<float>& data);

private:
    uint32_t _sampleRate;
    AudioFilterParams _filterParams;
    Biquad _biquad;
};
