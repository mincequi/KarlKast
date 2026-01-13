import 'package:get/get.dart';
import 'package:karl_kast/common/iso_octave_bands.dart';
import 'package:karl_kast/settings/setting.dart';
import 'package:karl_kast/settings/settings_provider.dart';

class SettingsController extends GetxController {
  final settingsProvider = Get.find<SettingsProvider>();

  Map<Setting, RxInt> settings = {
    Setting.lower_octave: 0.obs,
    Setting.upper_octave: (octaveBands.length - 1).obs,
    Setting.smoothing_tau: 200.obs // Default smoothing value
  };

  @override
  void onInit() {
    super.onInit();
    settingsProvider.onInit();
    settingsProvider.getSettings().then((fetchedSettings) {
      fetchedSettings.forEach((key, value) {
        if (settings.containsKey(key)) {
          settings[key]!.value = value;
        }
      });
    });
  }

  void setSetting(Setting setting, int value) {
    if (setting == Setting.lower_octave) {
      settings[setting]!.value =
          value.clamp(0, settings[Setting.upper_octave]!.value - 1);
    } else if (setting == Setting.upper_octave) {
      settings[setting]!.value = value.clamp(
          settings[Setting.lower_octave]!.value + 1, octaveBands.length - 1);
    } else {
      settings[setting]!.value = value;
    }

    settingsProvider.setSetting(setting, value);
  }
}
