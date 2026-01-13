#include "AlarmRepository.h"

#include <plog/Log.h>
#include <rpp/operators/debounce.hpp>
#include <rpp/operators/distinct_until_changed.hpp>
#include <rpp/operators/filter.hpp>
#include <rpp/operators/map.hpp>
#include <rpp/schedulers/current_thread.hpp>
#include <rpp/schedulers/new_thread.hpp>

#include <audio/AudioRepository.h>

using namespace rpp::operators;
using namespace rpp::schedulers;

AlarmRepository::AlarmRepository(const AudioRepository& audioRepository)
    : _audioRepository(audioRepository) {
}

rpp::dynamic_observable<bool> AlarmRepository::alarmObservable() {
    return _audioRepository.smoothedTimeDomainFeatures()
        | map([this](const TimeDomainFeatures& features) { return features.rms > -48; })
        | distinct_until_changed()
        | debounce(std::chrono::milliseconds(1000), new_thread{})
        | filter([](bool alarm) {
                 return alarm;
        });
}
