import 'package:flutter/material.dart';
import 'package:get/get.dart';
import 'package:karl_kast/common/iso_octave_bands.dart';
import 'package:karl_kast/settings/setting.dart';
import 'package:karl_kast/settings/settings_controller.dart';

class SettingsPage extends GetView<SettingsController> {
  @override
  Widget build(BuildContext context) {
    return SafeArea(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          mainAxisSize: MainAxisSize.min, // Wrap content
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text("Frequency range",
                style: Theme.of(context).textTheme.titleMedium),
            Obx(() {
              return RangeSlider(
                  min: 0,
                  max: octaveBands.length - 1,
                  values: RangeValues(
                      controller.settings[Setting.lower_octave]!.value
                          .toDouble(),
                      controller.settings[Setting.upper_octave]!.value
                          .toDouble()),
                  divisions: octaveBands.length - 1,
                  labels: RangeLabels(
                    "${octaveBands[controller.settings[Setting.lower_octave]!.value]} Hz",
                    "${octaveBands[controller.settings[Setting.upper_octave]!.value]} Hz",
                  ),
                  onChanged: (RangeValues values) {
                    controller.setSetting(
                        Setting.lower_octave, values.start.toInt());
                    controller.setSetting(
                        Setting.upper_octave, values.end.toInt());
                  });
            }),
            Text("Smoothing", style: Theme.of(context).textTheme.titleMedium),
            Obx(() {
              return Slider(
                min: 100,
                max: 1000,
                divisions: 9,
                value: controller.settings[Setting.smoothing_tau]!.value
                    .toDouble(),
                onChanged: (value) {
                  controller.setSetting(Setting.smoothing_tau, value.toInt());
                },
                label:
                    "${controller.settings[Setting.smoothing_tau]!.value} ms",
              );
            }),
          ],
        ),
      ),
    );
  }
}
