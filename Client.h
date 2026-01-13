#pragma once

#include <crow/websocket.h>

#include <rpp/fwd.hpp>
#include <rpp/observables/dynamic_observable.hpp>

class AlarmRepository;
class AudioRepository;

class Client {
public:
    Client(crow::websocket::connection* connection,
           const AlarmRepository& alarmRepository,
           const AudioRepository& audioRepository);
    ~Client();

private:
    friend class ClientRepository;

    crow::websocket::connection* _connection = nullptr;

    const AlarmRepository& _alarmRepository;
    rpp::composite_disposable_wrapper _alarmDisposable;

    const AudioRepository& _audioRepository;
    rpp::composite_disposable_wrapper _levelDisposable;
};
