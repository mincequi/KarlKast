#pragma once

#include <CrowApp.h>

class SettingsRepository;

class SettingsRoute {
public:
    SettingsRoute(CrowApp& app,
                  SettingsRepository& settingsRepository);

private:
    SettingsRepository& _settingsRepository;
};
