#pragma once

#include <rpp/observables/dynamic_observable.hpp>

class AudioRepository;

class AlarmRepository {
public:
    AlarmRepository(const AudioRepository& audioRepository);

    rpp::dynamic_observable<bool> alarmObservable();

private:
    const AudioRepository& _audioRepository;
};
