#include "ConsoleUi.h"

#include <iostream>

#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <plog/Record.h>
#include <plog/Util.h>
#include <plog/Formatters/FuncMessageFormatter.h>

#include <audio/AudioUtil.h>
#include <audio/TimeDomainFeatures.h>
#include <settings/OctaveBands.h>

using namespace ftxui;
using namespace plog;

ConsoleUi::ConsoleUi() :
    _canvas(80, 24) {
}

void ConsoleUi::feed(const TimeDomainFeatures& timeDomainFeatures) {
    std::lock_guard<std::mutex> lock(_mutex);

    _dataPoints.push_back(timeDomainFeatures);
    while (_dataPoints.size() > 150) {
        _dataPoints.pop_front();
    }
    if (_dataPoints.size() < 2) {
        return; // Not enough data points to draw
    }

    _minDelta = std::numeric_limits<int>::max();
    for (const auto& f :_dataPoints) {
        float delta = f.peak - f.rms;
        if (delta < _minDelta) {
            _minDelta = delta;
        }
    }

    render();
}

ftxui::Color::Palette16 ConsoleUi::colorForDb(int db) {
    if (db < 12) return ftxui::Color::Red;      // 0 - 12 dB
    if (db < 24) return ftxui::Color::Yellow;   // 12 - 24 dB
    if (db < 48) return ftxui::Color::Green;    // 24 - 48 dB
    /*if (db < 15)*/ return ftxui::Color::GrayDark;
}

ftxui::Color::Palette16 ConsoleUi::colorForSeverity(plog::Severity severity) {
    switch (severity) {
    case plog::debug: return ftxui::Color::GrayLight;
    case plog::info: return ftxui::Color::White;
    case plog::warning: return ftxui::Color::Yellow;
    case plog::error: return ftxui::Color::Red;
    case plog::fatal: return ftxui::Color::RedLight;
    default: return ftxui::Color::GrayDark;
    }
}

void ConsoleUi::write(const plog::Record& record) {
    //return;

    util::nstring str = FuncMessageFormatter::format(record);
    std::lock_guard<std::mutex> lock(_mutex);

    // insert text at front of vector
    _logLines.insert(_logLines.begin(), text(str) | color(colorForSeverity(record.getSeverity())));
    while (_logLines.size() > 20) {
        _logLines.pop_back();
    }

    render();
}

void ConsoleUi::render() {
    Canvas levelCanvas(150, 72);
    for (size_t i = 0; i < _dataPoints.size()-1; ++i) {
        levelCanvas.DrawPointLine(i, -(_dataPoints[i].rms + _minDelta),
                              i + 1, -(_dataPoints[i + 1].rms + _minDelta),
                              colorForDb(-(_dataPoints[i].rms + _minDelta)));
    }
    auto levelView = canvas(&levelCanvas) | border;

    Canvas zcrCanvas(150, (octaveBands.size()-1) * 2);
    for (size_t i = 0; i < _dataPoints.size()-1; ++i) {
        zcrCanvas.DrawPointLine(i, 2 * (octaveBands.size() - 1) - round(2 * _dataPoints[i].zcr),
                                i + 1, 2 * (octaveBands.size() - 1) - round(2 * _dataPoints[i + 1].zcr));
                                //colorForDb(-(_dataPoints[i].zcr)));
    }
    auto zcrView = canvas(&zcrCanvas) | border;
    auto logView = vbox(_logLines) | size(HEIGHT, LESS_THAN, 15);

    auto document = vbox({
        levelView,
        //separator(),
        zcrView,
        logView
    });

    // Create and render screen
    auto screen = Screen::Create(Dimension::Fit(document), Dimension::Fit(document));
    Render(screen, document);
    std::cout << screen.ResetPosition();
    screen.Print();
}
