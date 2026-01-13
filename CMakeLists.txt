cmake_minimum_required(VERSION 3.5)

project(KarlKast LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(KarlKast main.cpp
  AudioRoute.h
  AudioRecorder.h AudioRecorder.cpp
  ClientRepository.h ClientRepository.cpp
  Client.h Client.cpp
  settings/SettingsRoute.h settings/SettingsRoute.cpp
  settings/SettingsRepository.h settings/SettingsRepository.cpp
  settings/OctaveBands.h
  CrowApp.h
  audio/AudioFilter.h audio/AudioFilter.cpp
  audio/Biquad.h audio/Biquad.cpp
  audio/AudioFilterType.h
  audio/AudioDsp.h audio/AudioDsp.cpp
  audio/AudioFilterParams.h
  audio/AudioRepository.h audio/AudioRepository.cpp
  audio/AudioUtil.h audio/AudioUtil.cpp
  audio/TimeDomainFeatures.h
  console/ConsoleUi.h console/ConsoleUi.cpp
  alarm/AlarmRepository.h alarm/AlarmRepository.cpp
)

include(cmake/CPM.cmake)

# Add dependencies
CPMAddPackage("gh:ArthurSonzogni/FTXUI@6.1.9")
CPMAddPackage("gh:CrowCpp/Crow@1.2.1.2")
CPMAddPackage("gh:SergiusTheBest/plog#1.1.10")
CPMAddPackage("gh:mackron/miniaudio#0.11.22")
CPMAddPackage("gh:victimsnino/ReactivePlusPlus@2.2.2")
CPMAddPackage("gh:getml/reflect-cpp@0.19.0")

# Link libraries
target_link_libraries(KarlKast
PRIVATE
  Crow::Crow
  RPP::rpp
  ftxui::dom
  ftxui::screen
  plog::plog
  miniaudio
  reflectcpp
)

# Set include directories
target_include_directories(KarlKast
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

include(GNUInstallDirs)
install(TARGETS KarlKast
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
