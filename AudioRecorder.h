#pragma once

#include <functional>
#include <miniaudio.h>
#include <rpp/observables/dynamic_observable.hpp>
#include <rpp/subjects/publish_subject.hpp>

class SettingsRepository;

class AudioRecorder {
public:
    AudioRecorder(const SettingsRepository& settingsRepository);
    ~AudioRecorder();

    // This operator should be called to capture audio.
    void operator()() {
        auto result = ma_device_start(&device);
        if (result != MA_SUCCESS) {
            ma_device_uninit(&device);
            //printf("Failed to start device.\n");
            return;
        }

        //printf("Press Enter to stop recording...\n");
        //getchar();
    }

    inline rpp::dynamic_observable<std::vector<float>> audio() const {
        return _audioSubject.get_observable();
    }

private:
    static void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frameCount) {
        auto* self = static_cast<AudioRecorder*>(device->pUserData);

        std::vector<float> floatBuffer;
        floatBuffer.reserve(frameCount);

        const int16_t* inputBuffer = static_cast<const int16_t*>(input);
        std::transform(inputBuffer, inputBuffer + frameCount, std::back_inserter(floatBuffer), [](int16_t sample) {
            return static_cast<float>(sample) / 32768.0f;
        });
        self->_audioSubject.get_observer().on_next(std::move(floatBuffer));
    }

    const SettingsRepository& _settingsRepository;

    rpp::subjects::publish_subject<std::vector<float>> _audioSubject;

    ma_device device;
};
