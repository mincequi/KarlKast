import 'package:get/get.dart';
import 'package:karl_kast/common/iso_octave_bands.dart';

import 'setting.dart';

class SettingsProvider extends GetConnect {
  final String baseUrl = 'http://localhost:8030';

  Map<Setting, RxInt> settings = {
    Setting.lower_octave: 0.obs,
    Setting.upper_octave: (octaveBands.length - 1).obs,
    Setting.smoothing_tau: 200.obs, // Default smoothing value
  };

  @override
  void onInit() {
    httpClient.baseUrl = baseUrl;

    get('/settings').then((response) {
      if (response.status.hasError) {
        print('Error fetching settings: ${response.statusText}');
        return;
      }
      final data = response.body;
      if (data != null && data is Map<String, dynamic>) {
        data.forEach((key, value) {
          Setting setting = Setting.values.firstWhere((e) => e.name == key);
          if (settings.containsKey(setting)) {
            settings[setting]!.value = value;
          }
        });
      } else {
        print('Invalid settings data format');
      }
    }).catchError((error) {
      print('Error fetching settings: $error');
    });
  }

  Future<Map<Setting, int>> getSettings() async {
    final response = await get('/settings');
    if (response.status.hasError) {
      print('Error fetching settings: ${response.statusText}');
      return {};
    }
    final data = response.body;
    if (data is Map<String, dynamic>) {
      return data.map((key, value) =>
          MapEntry(Setting.values.firstWhere((e) => e.name == key), value));
    } else {
      print('Invalid settings data format');
      return {};
    }
  }

  Future<void> setSetting(Setting setting, int value) {
    return post('/settings/${setting.name}/${value.toString()}', "")
        .catchError((error) {
      print('Error setting ${setting.name}: $error');
    });
  }
}
