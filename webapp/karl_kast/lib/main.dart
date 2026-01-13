import 'package:flutter/material.dart';
import 'package:get/get.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:karl_kast/audio_binding.dart';
import 'package:karl_kast/audio_page.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();
  runApp(GetMaterialApp(
    title: 'KarlKast',
    themeMode: ThemeMode.dark,
    darkTheme: ThemeData(
      brightness: Brightness.dark,
      textTheme: GoogleFonts.josefinSansTextTheme(
        ThemeData.dark().textTheme,
      ),
    ),
    //home: MyHomePage(),
    home: AudioPage(),
    initialBinding: AudioBinding(),
  ));
}
