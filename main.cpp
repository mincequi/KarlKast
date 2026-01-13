#include <crow.h>
#include <crow/middlewares/CORS.h>

#include <plog/Init.h>
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/FuncMessageFormatter.h>

#include "AudioRecorder.h"
#include "AudioRoute.h"
#include <alarm/AlarmRepository.h>
#include <audio/AudioDsp.h>
#include <audio/AudioRepository.h>
#include <console/ConsoleUi.h>
#include <settings/SettingsRepository.h>
#include <settings/SettingsRoute.h>

static uint16_t port = 8030;

int main() {
    ConsoleUi consoleUi;
    static plog::ConsoleAppender<plog::FuncMessageFormatter> consoleAppender;
    plog::init(plog::debug, &consoleUi);

    crow::App<crow::CORSHandler> app;

    SettingsRepository settingsRepository;
    AudioRepository audioRepository(settingsRepository);
    audioRepository.smoothedTimeDomainFeatures().subscribe([&](const auto& timeDomainFeatures) {
        consoleUi.feed(timeDomainFeatures);
    });
    AlarmRepository alarmRepository(audioRepository);
    alarmRepository.alarmObservable().subscribe([&](bool alarm) {
        if (alarm) {
            LOGW << "Alarm triggered!";
        } else {
            LOGD << "Alarm cleared.";
        }
    });

    ClientRepository clientRepository(audioRepository, alarmRepository);
    AudioRoute audioRoute(app, clientRepository, audioRepository);
    SettingsRoute settingsRoute(app, settingsRepository);
    AudioRecorder audioRecorder(settingsRepository);
    AudioDsp audioDsp(settingsRepository);
    audioRecorder.audio().subscribe([&](const std::vector<float>& audioData) {
        audioDsp.process(const_cast<std::vector<float>&>(audioData));
        clientRepository.broadcastAudio(audioData);
        audioRepository.feed(audioData);
    });
    audioRecorder();

    app.port(port)
        .multithreaded()
        .run();
}
