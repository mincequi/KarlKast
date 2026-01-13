// services/audio_stream_service.dart
import 'dart:math';
import 'dart:typed_data';
import 'package:get/get.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

class AudioService extends GetxService {
  final _url = 'ws://localhost:8030/audio';
  late WebSocketChannel _channel;

  Function(Uint8List bytes)? onData;
  Function(String message)? onMessage;

  void connect() {
    _channel = WebSocketChannel.connect(Uri.parse(_url));
    _channel.stream.listen((data) {
      if (data is String && onMessage != null) {
        onMessage!(data);
      } else if (data is Uint8List && onData != null) {
        // Call the onData callback with the received Uint8List
        onData!(data);
      }
    }, onError: (error) {
      print('WebSocket error: $error');
    }, onDone: () {
      print('WebSocket closed');
    });
  }

  void disconnect() {
    _channel.sink.close();
  }
}
