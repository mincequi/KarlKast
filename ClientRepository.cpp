#include "ClientRepository.h"

#include <audio/AudioRepository.h>

ClientRepository::ClientRepository(const AudioRepository& audioRepository,
                                   const AlarmRepository& alarmRepository)
    : _audioRepository(audioRepository),
      _alarmRepository(alarmRepository) {
}

void ClientRepository::addClient(crow::websocket::connection* conn) {
    std::lock_guard<std::mutex> lock(_clientsMutex);

    _clients.emplace_back(Client(conn, _alarmRepository, _audioRepository));
}

void ClientRepository::removeClient(crow::websocket::connection* conn) {
    std::lock_guard<std::mutex> lock(_clientsMutex);

    auto it = std::find_if(_clients.begin(), _clients.end(), [&](const Client& client) {
        return client._connection == conn;
    });
    if (it != _clients.end()) {
        it->_connection->close();
        _clients.erase(it);
    }
}

void ClientRepository::broadcastAudio(const std::vector<float>& audioData) {
    std::lock_guard<std::mutex> lock(_clientsMutex);
    if (_clients.empty() || audioData.empty()) {
        return;
    }
    // Convert float audio data to int16_t
    size_t frameCount = audioData.size();
    std::vector<int16_t> data(frameCount);
    for (size_t i = 0; i < frameCount; ++i) {
        data[i] = static_cast<int16_t>(audioData[i] * 32767.0f); // Scale to int16_t range
    }

    for (const auto& client : _clients) {
        //client._connection->send_text("hello");
        client._connection->send_binary(std::string(reinterpret_cast<const char*>(data.data()), frameCount * sizeof(int16_t)));
    }
}
