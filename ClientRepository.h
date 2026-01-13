#pragma once

#include <list>

#include <crow/websocket.h>

#include "Client.h"

class AudioRepository;

class ClientRepository {
public:
    ClientRepository(const AudioRepository& audioRepository,
                     const AlarmRepository& alarmRepository);

    void addClient(crow::websocket::connection* conn);
    void removeClient(crow::websocket::connection* conn);

    void broadcastAudio(const std::vector<float>& audioData);

private:
    const AudioRepository& _audioRepository;
    const AlarmRepository& _alarmRepository;

    std::list<Client> _clients;
    std::mutex _clientsMutex;
};
