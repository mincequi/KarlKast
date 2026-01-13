#include "AudioRepository.h"

#include <audio/AudioUtil.h>
#include <settings/SettingsRepository.h>

#include <rpp/operators/combine_latest.hpp>
#include <rpp/operators/map.hpp>

using namespace rpp::operators;

AudioRepository::AudioRepository(const SettingsRepository& settingsRepository) :
    _settingsRepository(settingsRepository),
    _timeDomainFeaturesSubject(settingsRepository.historyLength),
    _smoothedTimeDomainFeaturesSubject(settingsRepository.historyLength) {

    _timeDomainFeaturesSubject.get_observable()
        | combine_latest([this](const TimeDomainFeatures& features, int tau) {
              const float deltaT = (float)features.frameSize / static_cast<float>(_settingsRepository.sampleRate/1000);
              const float alpha = deltaT / (tau + deltaT);

              return TimeDomainFeatures{
                  .peak = (int)round(alpha * features.peak + (1.0f - alpha) * _smoothedTimeDomainFeatures.peak),
                  .rms = (int)round(alpha * features.rms + (1.0f - alpha) * _smoothedTimeDomainFeatures.rms),
                  .ste = round(alpha * features.ste + (1.0f - alpha) * _smoothedTimeDomainFeatures.ste),
                  .zcr = alpha * features.zcr + (1.0f - alpha) * _smoothedTimeDomainFeatures.zcr,
                  .frameSize = features.frameSize
              };
          }, _settingsRepository.smoothingTau())
        | subscribe(
              [this](const TimeDomainFeatures& smoothedFeatures) {
                  _smoothedTimeDomainFeatures = smoothedFeatures;
                  _smoothedTimeDomainFeaturesSubject.get_observer().on_next(smoothedFeatures);
              },
              [this](const std::exception_ptr& e) {
                  _smoothedTimeDomainFeaturesSubject.get_observer().on_error(e);
              },
              [this]() {
                  _smoothedTimeDomainFeaturesSubject.get_observer().on_completed();
              });
}

void AudioRepository::feed(const std::vector<float>& audioData) {
    timeDomainFeatures(audioData);
}

int AudioRepository::toDb(float value) {
    // Convert float sample to dB
    if (value == 0) return -100; // Avoid division by zero
    return static_cast<int>(round(20.0f * std::log10(std::fabs(value))));
}

void AudioRepository::timeDomainFeatures(const std::vector<float>& audioData) const {
    float peak = 0.0f;
    float ste = 0.0;
    for (const auto& sample : audioData) {
        const float absSample = std::fabs(sample);
        ste += sample * sample;
        if (absSample > peak) {
            peak = absSample;
        }
    }
    const float rms = std::sqrt(ste / audioData.size());

    _timeDomainFeaturesSubject.get_observer().on_next(TimeDomainFeatures{
        .peak = toDb(peak),
        .rms = toDb(rms),
        .ste = ste,
        .zcr = frequencyToLogIndex(zeroCrossingRate(audioData)),
        .frameSize = static_cast<int>(audioData.size()),
    });
}

int AudioRepository::zeroCrossingRate(const std::vector<float>& audioData) const {
    if (audioData.size() < 2) {
        return 0; // Not enough data to compute zero crossing rate
    }

    int zeroCrossings = 0;
    for (size_t i = 1; i < audioData.size(); ++i) {
        if ((audioData[i - 1] >= 0 && audioData[i] < 0) /*|| (audioData[i - 1] < 0 && audioData[i] >= 0)*/) {
            ++zeroCrossings;
        }
    }
    return (_settingsRepository.sampleRate * zeroCrossings) / (audioData.size() - 1);
}
