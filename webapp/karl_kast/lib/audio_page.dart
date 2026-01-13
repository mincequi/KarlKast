// views/audio_stream_page.dart
import 'package:flutter/material.dart';
import 'package:get/get.dart';
import 'package:karl_kast/audio_controller.dart';
import 'package:karl_kast/audio/audio_level_widget.dart';
import 'package:karl_kast/settings/settings_page.dart';

class AudioPage extends GetView<AudioController> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('KarlKast'),
        actions: [
          IconButton(
            icon: Icon(Icons.settings),
            onPressed: () => _openSettings(context),
          ),
        ],
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const AudioLevelWidget(),
            //const AudioZcrWidget(),
            const Spacer(),
            Icon(Icons.headphones, size: 80),
            SizedBox(height: 20),
            Text('Streaming audio...'),
            Obx(() => Text(
                "full: ${controller.stat.value.full} exhaust:${controller.stat.value.exhaust}")),
            Obx(() => ElevatedButton(
                onPressed:
                    controller.isPlaying.value ? null : controller.onPressed,
                child: const Text(
                  'generate sine wave',
                )))
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: controller.isPlaying.value ? null : controller.onPressed,
        child: Obx(() => Icon(
              controller.isPlaying.value ? Icons.stop : Icons.play_arrow,
            )),
      ),
    );
  }

  void _openSettings(BuildContext context) {
    showModalBottomSheet(
      context: context,
      isScrollControlled: true, // Optional: makes sheet take full height
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.vertical(top: Radius.circular(2)),
      ),
      builder: (context) => SettingsPage(),
    );
  }
}
