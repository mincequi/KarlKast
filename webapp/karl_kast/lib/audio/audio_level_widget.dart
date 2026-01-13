import 'dart:math';

import 'package:flutter/material.dart';
import 'package:get/get.dart';

import 'package:fl_chart/fl_chart.dart';
import 'package:karl_kast/audio/audio_gauge.dart';
import 'package:karl_kast/audio/audio_level_controller.dart';
import 'package:karl_kast/common/constants.dart';
import 'package:karl_kast/common/iso_octave_bands.dart';

class AudioLevelWidget extends GetView<AudioLevelController> {
  const AudioLevelWidget({Key? key}) : super(key: key);

  final double _space = 1; // Space between bars

  @override
  Widget build(BuildContext context) {
    return AspectRatio(
      aspectRatio: 2.5,
      child: Padding(
        padding: const EdgeInsets.only(top: 0),
        child: Obx(
          () {
            final features = controller.features.value;
            return LayoutBuilder(
              builder: (context, constraints) {
                final barsWidth = (constraints.maxWidth - (151 * _space)) / 150;

                return Stack(children: [
                  BarChart(
                    duration: const Duration(milliseconds: 0),
                    BarChartData(
                        alignment: BarChartAlignment.end,
                        barTouchData: BarTouchData(
                          enabled: false,
                        ),
                        titlesData: const FlTitlesData(
                          show: false,
                        ),
                        gridData: const FlGridData(
                          show: false,
                        ),
                        borderData: FlBorderData(
                          show: false,
                        ),
                        barGroups: rmsData(barsWidth, features),
                        groupsSpace: _space),
                  ),
                  LineChart(
                    duration: const Duration(milliseconds: 0),
                    LineChartData(
                        minX: 0,
                        maxX: features.length.toDouble(),
                        minY: 0,
                        maxY: octaveBands.length.toDouble(),
                        lineTouchData: const LineTouchData(
                          enabled: false,
                        ),
                        titlesData: const FlTitlesData(
                          show: false,
                        ),
                        gridData: const FlGridData(
                          show: false,
                        ),
                        borderData: FlBorderData(
                          show: false,
                        ),
                        lineBarsData: [zcrData(barsWidth, features)]),
                  ),
                  Positioned(
                    top: 0,
                    right: 0,
                    width: constraints.maxHeight * 0.5,
                    height: constraints.maxHeight * 0.5,
                    child: AudioGauge(
                      yOffset: constraints.maxHeight * 0.25,
                    ),
                  ),
                ]);
              },
            );
          },
        ),
      ),
    );
  }

  List<BarChartGroupData> rmsData(
      double barsWidth, List<TimeDomainFeatures> features) {
    double minDeltaPeakRms = features.isNotEmpty
        ? features.map((f) => f.peak - f.rms).reduce(min)
        : 0.0;

    List<double> rmss =
        features.map((f) => max(f.rms + minDeltaPeakRms, dbThreshold)).toList();

    return List.generate(
        rmss.length,
        (index) => BarChartGroupData(
              x: 0,
              barsSpace: 0,
              // Create barRods based on the levels
              barRods: [
                BarChartRodData(
                  fromY: dbThreshold,
                  //toY: features[index].rms,
                  toY: rmss[index],
                  backDrawRodData: BackgroundBarChartRodData(
                      show: true,
                      fromY: dbThreshold,
                      toY: 0,
                      color: Colors.grey.shade900),
                  borderSide: null,
                  rodStackItems: [
                    BarChartRodStackItem(dbThreshold, min(-48, rmss[index]),
                        Colors.grey.shade700),
                    if (rmss[index] > -48)
                      BarChartRodStackItem(
                          -48, min(-24, rmss[index]), Colors.green.shade700),
                    if (rmss[index] > -24)
                      BarChartRodStackItem(
                          -24, min(-12, rmss[index]), Colors.yellow.shade700),
                    if (rmss[index] > -12)
                      BarChartRodStackItem(
                          -12, rmss[index], Colors.red.shade700),
                  ],
                  borderRadius: BorderRadius.zero,
                  width: barsWidth,
                )
              ],
            ));
  }

  LineChartBarData zcrData(double barWidth, List<TimeDomainFeatures> features) {
    return LineChartBarData(
      barWidth: barWidth * 0.5,
      color: Colors.grey.shade700,
      isStepLineChart: true,
      shadow: const Shadow(
        color: Colors.black,
        blurRadius: 0,
        offset: Offset(2, 2),
      ),
      spots: List.generate(features.length, (index) {
        final feature = features[index];
        return FlSpot(index.toDouble(), feature.zcr);
      }),
      isCurved: false,
      dotData: const FlDotData(show: false),
      belowBarData: BarAreaData(show: false),
    );
  }
}
