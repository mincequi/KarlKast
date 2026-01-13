#include "AudioRecorder.h"

#include <settings/SettingsRepository.h>

AudioRecorder::AudioRecorder(const SettingsRepository& settingsRepository) :
    _settingsRepository(settingsRepository) {
    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_capture);
    deviceConfig.capture.format   = ma_format_s16;
    deviceConfig.capture.channels = 1;
    deviceConfig.sampleRate       = settingsRepository.sampleRate;
    deviceConfig.dataCallback     = data_callback;
    deviceConfig.pUserData        = this;

    auto result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        //printf("Failed to initialize capture device.\n");
        return;
    }
}

AudioRecorder::~AudioRecorder() {
    ma_device_uninit(&device);
}
