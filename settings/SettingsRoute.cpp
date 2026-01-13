#include "SettingsRoute.h"

#include "SettingsRepository.h"

SettingsRoute::SettingsRoute(CrowApp& app,
                           SettingsRepository& settingsRepository)
    : _settingsRepository(settingsRepository) {
    CROW_ROUTE(app, "/settings")
    ([this]() {
        crow::json::wvalue response;
        for (const auto& setting : _settingsRepository.settings()) {
            response[setting.first] = setting.second;
        }
        return crow::response(200, response);
    });

    CROW_ROUTE(app, "/settings/<string>/<int>")
    .methods("POST"_method)
    ([this](const std::string& key, int value) {
        _settingsRepository.setSetting(key, value);
        return crow::response(200, "Setting updated");
    });
}
