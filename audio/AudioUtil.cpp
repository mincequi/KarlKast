#include "AudioUtil.h"

#include <settings/OctaveBands.h>

static const double totalRange = std::log(octaveBands.back() / octaveBands.front());

double frequencyToLogIndex(double freq) {
    if (freq <= octaveBands.front()) return 0;
    if (freq >= octaveBands.back()) return (octaveBands.size() - 1);

    const double ratio = freq / octaveBands.front();
    return std::log(ratio) / totalRange * (octaveBands.size() - 1.0);
}
