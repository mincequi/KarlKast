#include "AudioDsp.h"

#include <settings/OctaveBands.h>
#include <settings/SettingsRepository.h>

AudioDsp::AudioDsp(const SettingsRepository& settingsRepository)
    : _settingsRepository(settingsRepository) {
    _highPass.setSampleRate(settingsRepository.sampleRate);
    _lowPass.setSampleRate(settingsRepository.sampleRate);

    _settingsRepository.octaves().subscribe([this](const auto& octaves) {
        std::lock_guard<std::mutex> lock(_filtersMutex);

        _highPass.setFilterParams(AudioFilterParams{
            .type = AudioFilterType::HighPass,
            .f = octaveBands[octaves.first],
        });
        _lowPass.setFilterParams(AudioFilterParams{
            .type = AudioFilterType::LowPass,
            .f = octaveBands[octaves.second],
        });
    });
}

void AudioDsp::process(std::vector<float>& audio) {
    std::lock_guard<std::mutex> lock(_filtersMutex);

    if (!_highPass.isValid() || !_lowPass.isValid()) {
        return;
    }

    _highPass.process(audio);
    _lowPass.process(audio);
}
