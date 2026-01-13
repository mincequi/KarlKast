import 'dart:convert';

import 'package:get/get.dart';
import 'package:karl_kast/audio_service.dart';

class TimeDomainFeatures {
  double peak;
  double rms;
  double zcr;

  TimeDomainFeatures({
    required this.peak,
    required this.rms,
    required this.zcr,
  });

  TimeDomainFeatures.fromJson(Map<String, dynamic> json)
      : peak = double.tryParse(json['peak'].toString()) ?? 0.0,
        rms = double.tryParse(json['rms'].toString()) ?? 0.0,
        zcr = double.tryParse(json['zcr'].toString()) ?? 0.0;
}

class AudioLevelController extends GetxController {
  final _audioService = Get.find<AudioService>();

  // A buffer of up to 150 elements to store audio features
  final features = <TimeDomainFeatures>[].obs;

  @override
  void onInit() {
    super.onInit();

    _audioService.onMessage = (String message) {
      // Parse message as json
      var features_ = TimeDomainFeatures.fromJson(jsonDecode(message));
      if (features.length >= 150) {
        features.removeAt(0); // Remove the oldest level if buffer is full
      }
      features.add(features_);
    };
  }

  // Other methods and properties for the controller
}
