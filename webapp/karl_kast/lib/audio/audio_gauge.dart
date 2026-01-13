import 'package:flutter/material.dart';
import 'package:geekyants_flutter_gauges/geekyants_flutter_gauges.dart';
import 'package:get/get.dart';

import 'package:karl_kast/audio/audio_level_controller.dart';
import 'package:karl_kast/audio/audio_util.dart';
import 'package:karl_kast/common/constants.dart';
import 'package:karl_kast/common/iso_octave_bands.dart';
import 'package:karl_kast/common/value_unit_widget.dart';

class AudioGauge extends GetView<AudioLevelController> {
  const AudioGauge({
    this.yOffset = 0.0,
    super.key,
  });

  final double yOffset;

  @override
  Widget build(BuildContext context) {
    const barsWidth = 3.0;
    final db = AudioUtil.toDb(controller.features.value);
    final zcr = controller.features.value.isNotEmpty
        ? controller.features.value.last.zcr
        : 0.0;
    return Stack(
      children: [
        RadialGauge(
          radiusFactor: 1,
          valueBar: [
            RadialValueBar(
                value: db,
                valueBarThickness: barsWidth,
                radialOffset: -barsWidth / 2 - 0.5,
                color: AudioUtil.dbToColor(db)[700]!),
            RadialValueBar(
              value: zcr / octaveBands.length * dbThreshold.abs() +
                  dbThreshold, // Scale ZCR to fit in dB range
              valueBarThickness: barsWidth,
              radialOffset: barsWidth / 2 + 0.5,
              color: AudioUtil.zcrToColor(zcr)[700]!,
            ),
          ],
          track: RadialTrack(
              hideTrack: false,
              start: dbThreshold,
              end: 0,
              color: Colors.black,
              startAngle: -90,
              endAngle: 180,
              thickness: 2 * barsWidth + 3,
              trackStyle: TrackStyle(
                showPrimaryRulers: false,
                showSecondaryRulers: false,
                showLabel: false,
              )),
        ),
        Positioned(
          right: 6,
          top: yOffset + 6,
          child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.end,
              children: [
                ValueUnitWidget(
                    value: db, unit: "dB", colorFunction: AudioUtil.dbToColor),
                ValueUnitWidget(
                    value: octaveBands[zcr.round()].toDouble(), unit: "Hz"),
              ]),
        ),
      ],
    );
  }
}
