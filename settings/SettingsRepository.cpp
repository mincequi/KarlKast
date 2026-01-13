#include "SettingsRepository.h"

#include "OctaveBands.h"

SettingsRepository::SettingsRepository() :
    _octavesSubject(std::make_pair(0, octaveBands.size()-1)),
    _smoothingTau(500) {
    // Initialize default settings
    _settings["lower_octave"] = 0;
    _settings["upper_octave"] = octaveBands.size() - 1;
}

std::optional<int> SettingsRepository::setting(const std::string& key) const {
    auto it = _settings.find(key);
    if (it != _settings.end()) {
        return it->second;
    }
    return std::nullopt;
}

void SettingsRepository::setSetting(const std::string& key, int value) {
    _settings[key] = value;

    if (key == "lower_octave" || key == "upper_octave") {
        int lower = _settings["lower_octave"];
        int upper = _settings["upper_octave"];

        _octavesSubject.get_observer().on_next(std::make_pair(lower, upper));
    } else if (key == "smoothing_tau") {
        _smoothingTau.get_observer().on_next(value);
    }
}
