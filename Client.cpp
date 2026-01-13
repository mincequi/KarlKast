#include "Client.h"

#include <alarm/AlarmRepository.h>
#include <audio/AudioRepository.h>

#include <rfl/json.hpp>

Client::Client(crow::websocket::connection* conn,
               const AlarmRepository& alarmRepository,
               const AudioRepository& audioRepository) :
    _connection(conn),
    _alarmRepository(alarmRepository),
    _audioRepository(audioRepository) {

    //_alarmDisposable = _alarmRepository.alarmObservable().subscribe_with_disposable([this](bool alarm) {
    //    _connection->send_text(rfl::json::write(alarm));
    //});

    // Subscribe to audio levels from the repository
    _levelDisposable = audioRepository.smoothedTimeDomainFeatures().subscribe_with_disposable([this](const auto& timeDomainFeatures) {
        _connection->send_text(rfl::json::write(timeDomainFeatures.round()));
    });
}

Client::~Client() {
    _levelDisposable.dispose();
}
