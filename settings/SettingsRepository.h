#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include <rpp/observables/dynamic_observable.hpp>
#include <rpp/subjects/behavior_subject.hpp>

class SettingsRepository {
public:
    SettingsRepository();

    std::optional<int> setting(const std::string& key) const;
    void setSetting(const std::string& key, int value);

    inline rpp::dynamic_observable<std::pair<int,int>> octaves() const {
        return _octavesSubject.get_observable();
    }

    inline rpp::dynamic_observable<int> smoothingTau() const {
        return _smoothingTau.get_observable();
    }

    const int32_t sampleRate = 8000;
    const int32_t historyLength = 150;

    inline const std::unordered_map<std::string, int>& settings() const {
        return _settings;
    }

private:
    std::unordered_map<std::string, int> _settings;

    rpp::subjects::behavior_subject<std::pair<int,int>> _octavesSubject;
    rpp::subjects::behavior_subject<int> _smoothingTau;
};
