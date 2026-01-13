#pragma once

#include <unordered_set>
#include <plog/Log.h>

#include <audio/AudioRepository.h>
#include "ClientRepository.h"
#include "CrowApp.h"

class AudioRoute {
public:
    AudioRoute(CrowApp& app,
               ClientRepository& clientRepository,
               const AudioRepository& audioRepository) :
        _clientRepository(clientRepository) {
        CROW_WEBSOCKET_ROUTE(app, "/audio")
            .onopen([&](crow::websocket::connection& conn) {
                LOGI << "new client: " << conn.get_remote_ip();
                _clientRepository.addClient(&conn);
            })
            .onclose([&](crow::websocket::connection& conn, const std::string& reason, uint16_t) {
                LOGI << "client disconnected: " << conn.get_remote_ip() << " reason: " << reason;
                _clientRepository.removeClient(&conn);
            })
            // message from API client shall be connection agnostic
            .onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
            })
            .onerror([&](crow::websocket::connection& conn, const std::string& data) {
                LOGE << "websocket error: client: " << conn.get_remote_ip() << " error: " << data;
                _clientRepository.removeClient(&conn);
            });
    }

private:
    ClientRepository& _clientRepository;
};
