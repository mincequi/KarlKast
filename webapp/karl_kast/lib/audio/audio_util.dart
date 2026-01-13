import 'dart:math';

import 'package:flutter/material.dart';
import 'package:karl_kast/audio/audio_level_controller.dart';
import 'package:karl_kast/common/constants.dart';

class AudioUtil {
  static MaterialColor dbToColor(double db) {
    if (db < -48) {
      return Colors.grey;
    } else if (db < -24) {
      return Colors.green;
    } else if (db < -12) {
      return Colors.yellow;
    } else {
      return Colors.red;
    }
  }

  static MaterialColor zcrToColor(double zcr) {
    if (zcr < 15.0) {
      return Colors.grey;
    } else if (zcr < 23.0) {
      return Colors.green;
    } else {
      return Colors.grey;
    }
  }

  static double toDb(List<TimeDomainFeatures> features) {
    double minDeltaPeakRms = features.isNotEmpty
        ? features.map((f) => f.peak - f.rms).reduce(min)
        : 0.0;

    return max(features.last.rms + minDeltaPeakRms, dbThreshold);
  }
}
