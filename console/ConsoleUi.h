#pragma once

#include <deque>

#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <plog/Appenders/IAppender.h>

#include <audio/TimeDomainFeatures.h>

namespace plog {
class Record;
}

class ConsoleUi : public plog::IAppender {
public:
    ConsoleUi();

    void feed(const TimeDomainFeatures& db);

private:
    static ftxui::Color::Palette16 colorForDb(int db);
    static ftxui::Color::Palette16 colorForSeverity(plog::Severity severity);

    void write(const plog::Record& record) override;

    void render();

    std::deque<TimeDomainFeatures> _dataPoints;
    int _minDelta = std::numeric_limits<int>::max();

    std::vector<ftxui::Element> _logLines;

    ftxui::Canvas _canvas;
    std::mutex _mutex;
};
