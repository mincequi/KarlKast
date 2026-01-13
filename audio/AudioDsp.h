#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "AudioFilter.h"

class SettingsRepository;

class AudioDsp {
public:
    AudioDsp(const SettingsRepository& settingsRepository);

    void process(std::vector<float>& audio);

private:
    const SettingsRepository& _settingsRepository;

    AudioFilter _lowPass;
    AudioFilter _highPass;
    std::mutex _filtersMutex;
};
