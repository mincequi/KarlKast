import 'dart:math' as math;
import 'dart:typed_data';

import 'package:get/get.dart';
import 'package:mp_audio_stream/mp_audio_stream.dart';
import 'package:karl_kast/audio_service.dart';

class AudioController extends GetxController {
  static const int sampleRate = 8000;
  static const int channelCount = 1;

  final _audioStream = getAudioStream();

  Rx<AudioStreamStat> stat = Rx<AudioStreamStat>(AudioStreamStat.empty());
  RxBool isPlaying = RxBool(false);

  @override
  void onInit() {
    super.onInit();

    _audioStream.init(
        sampleRate: sampleRate,
        channels: 1,
        bufferMilliSec: 300,
        waitingBufferMilliSec: 120);
    //_audioStream.resume();
    final audioService = Get.find<AudioService>();
    audioService.onData = (Uint8List bytes) {
      //_audioStream.resume();
      const double scale = 1.0 / 32768.0;

      final Int16List int16Samples = bytes.buffer.asInt16List();
      final Float32List output = Float32List(int16Samples.length);

      for (int i = 0; i < int16Samples.length; i++) {
        output[i] = int16Samples[i] * scale;
      }

      _audioStream.push(output);
    };
    audioService.connect();
  }

  @override
  void onClose() {
    final audioService = Get.find<AudioService>();
    audioService.disconnect();
    _audioStream.uninit();
    super.onClose();
  }

  void onPressed() async {
    // We need to resume the audio stream by user interaction on web platform.
    // This is required to activate `AudioContext`.
    _audioStream.resume();
  }
}
