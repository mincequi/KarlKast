#include "AudioFilter.h"

AudioFilter::AudioFilter() {
}

bool AudioFilter::isValid() const {
    return _biquad.isValid();
}

AudioFilterParams AudioFilter::filterParams() const {
    return _filterParams;
}

void AudioFilter::setFilterParams(const AudioFilterParams& params) {
    if (_filterParams == params) {
        return;
    }

    _filterParams = params;
    _biquad.setFilterParams(params);
}

//void AudioFilter::setCascadeCount(uint8_t count) {
//    _biquads.setCascadeCount(count);
//}

void AudioFilter::setSampleRate(uint32_t rate) {
    _sampleRate = rate;
    _biquad.setSampleRate(rate);
}

void AudioFilter::process(std::vector<float>& data) {
    _biquad.process(data);
}
