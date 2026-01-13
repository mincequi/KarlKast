// bindings/audio_binding.dart
import 'package:get/get.dart';
import 'package:karl_kast/audio_controller.dart';
import 'package:karl_kast/audio/audio_level_controller.dart';
import 'package:karl_kast/audio_service.dart';
import 'package:karl_kast/settings/settings_controller.dart';
import 'package:karl_kast/settings/settings_provider.dart';

class AudioBinding extends Bindings {
  @override
  void dependencies() {
    Get.put<AudioService>(AudioService());
    Get.put<AudioController>(AudioController());
    Get.put<AudioLevelController>(AudioLevelController());
    Get.put<SettingsProvider>(SettingsProvider());
    Get.put<SettingsController>(SettingsController());
  }
}
