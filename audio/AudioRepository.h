#pragma once

#include <cstddef>
#include <deque>

#include <rpp/observables/dynamic_observable.hpp>
#include <rpp/subjects/behavior_subject.hpp>
#include <rpp/subjects/replay_subject.hpp>

#include <audio/TimeDomainFeatures.h>

class SettingsRepository;

class AudioRepository {
public:
    AudioRepository(const SettingsRepository& settingsRepository);

    void feed(const std::vector<float>& audioData);

    inline rpp::dynamic_observable<TimeDomainFeatures> timeDomainFeatures() const {
        return _timeDomainFeaturesSubject.get_observable();
    }

    inline rpp::dynamic_observable<TimeDomainFeatures> smoothedTimeDomainFeatures() const {
        return _smoothedTimeDomainFeaturesSubject.get_observable();
    }

private:
    static int toDb(float value);

    const SettingsRepository& _settingsRepository;

    void timeDomainFeatures(const std::vector<float>& audioData) const;
    rpp::subjects::replay_subject<TimeDomainFeatures> _timeDomainFeaturesSubject;
    rpp::subjects::replay_subject<TimeDomainFeatures> _smoothedTimeDomainFeaturesSubject;
    TimeDomainFeatures _smoothedTimeDomainFeatures;

    int zeroCrossingRate(const std::vector<float>& audioData) const;
};
