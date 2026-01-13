#include "Biquad.h"

#include <cmath>

Biquad::Biquad() {
    _history.resize(1);
}

void Biquad::setCascadeCount(std::uint8_t count) {
    _history.resize(count);
}

void Biquad::setSampleRate(std::uint32_t rate) {
    if (_sampleRate == rate) {
        return;
    }
    _sampleRate = rate;
    updateCoeffs();
}

AudioFilterParams Biquad::filterParams() const {
    return _filterParams;
}

void Biquad::setFilterParams(const AudioFilterParams& filter) {
    if (_filterParams == filter) {
        return;
    }
    _filterParams = filter;
    updateCoeffs();
}

void Biquad::process(std::vector<float>& data) {
    // Iterate cascades
    for (std::size_t cascadeIndex = 0; cascadeIndex < _history.size(); ++cascadeIndex) {
        auto& h = _history.at(cascadeIndex);
        for (auto& x : data) {
            double y = 0.0;
            y += _coeffs.b0 * x;
            y += _coeffs.b1 * h.x1;
            y += _coeffs.b2 * h.x2;
            y -= _coeffs.a1 * h.y1;
            y -= _coeffs.a2 * h.y2;

            h.y2 = h.y1;
            h.y1 = y;
            h.x2 = h.x1;
            h.x1 = x;

            x = y;
        }
    }
}

bool Biquad::isValid() const {
    return (_sampleRate > 0 && _filterParams.isValid());
}

bool Biquad::updateCoeffs() {
    if (_sampleRate == 0 || !_filterParams.isValid()) {
        return false;
    }

    double b0 = 0.0;
    double b1 = 0.0;
    double b2 = 0.0;
    double a0 = 0.0;
    double a1 = 0.0;
    double a2 = 0.0;

    switch (_filterParams.type) {
    case AudioFilterType::Gain:
        a0 = 1.0;
        b0 = pow(10, _filterParams.g / 20.0); // Gain needs division by 20
        break;
    case AudioFilterType::Peaking: {
        double A = pow(10, _filterParams.g / 40.0);
        double w0 = 2*M_PI*_filterParams.f/_sampleRate;
        double alpha = sin(w0)*0.5/_filterParams.q;
        double alpha1 = alpha*A;
        double alpha2 = alpha/A;

        a0 = 1.0 + alpha2;
        b0 = ( 1.0 + alpha1 ) / a0;
        b1 = (-2.0 * cos(w0)) / a0;
        b2 = ( 1.0 - alpha1 ) / a0;
        a1 = b1;
        a2 = ( 1.0 - alpha2 ) / a0;
        break;
    }
    case AudioFilterType::LowPass: {
        double w0 = 2 * M_PI *_filterParams.f / _sampleRate;
        double alpha = sin(w0) * 0.5 / _filterParams.q;
        double cosw0 = cos(w0);

        a0 = 1.0 + alpha;
        b1 = ( 1.0 - cosw0 ) / a0;
        b0 = b1 * 0.5;
        b2 = b0;
        a1 = (-2.0 * cosw0 ) / a0;
        a2 = ( 1.0 - alpha ) / a0;
        break;
    }
    case AudioFilterType::HighPass: {
        double w0 = 2 * M_PI * _filterParams.f / _sampleRate;
        double alpha = sin(w0) * 0.5 / _filterParams.q;
        double cosw0 = cos(w0);

        a0    = 1.0 + alpha;
        b1 = -( 1.0 + cosw0 ) / a0;
        b0 = b1 * -0.5;
        b2 = b0;
        a1 = (-2.0 * cosw0) / a0;
        a2 = ( 1.0 - alpha  ) / a0;
        break;
    }
    case AudioFilterType::LowShelf: {
        double A = pow(10, _filterParams.g / 40.0);
        double w0 = 2 * M_PI * _filterParams.f / _sampleRate;
        double cosW0 = cos(w0);
        double alpha = sin(w0) * 0.5 / _filterParams.q;
        double sqrtAalpha2 = 2.0 * sqrt(A) * alpha;

        a0 = (A+1) + (A-1)*cosW0 + sqrtAalpha2;
        b0 =    A*( (A+1) - (A-1)*cosW0 + sqrtAalpha2) / a0;
        b1 =  2*A*( (A-1) - (A+1)*cosW0              ) / a0;
        b2 =    A*( (A+1) - (A-1)*cosW0 - sqrtAalpha2) / a0;
        a1 =   -2*( (A-1) + (A+1)*cosW0              ) / a0;
        a2 =      ( (A+1) + (A-1)*cosW0 - sqrtAalpha2) / a0;
        break;
    }
    case AudioFilterType::HighShelf: {
        double A = pow(10, _filterParams.g / 40.0);
        double w0 = 2 * M_PI * _filterParams.f / _sampleRate;
        double cosW0 = cos(w0);
        double alpha = sin(w0)*0.5 / _filterParams.q;
        double sqrtAalpha2 = 2.0 * sqrt(A) * alpha;

        a0 = (A+1) - (A-1)*cosW0 + sqrtAalpha2;
        b0 =    A*( (A+1) + (A-1)*cosW0 + sqrtAalpha2) / a0;
        b1 = -2*A*( (A-1) + (A+1)*cosW0              ) / a0;
        b2 =    A*( (A+1) + (A-1)*cosW0 - sqrtAalpha2) / a0;
        a1 =    2*( (A-1) - (A+1)*cosW0              ) / a0;
        a2 =      ( (A+1) - (A-1)*cosW0 - sqrtAalpha2) / a0;
        break;
    }
    case AudioFilterType::Bypass:
    case AudioFilterType::AllPass:
        return false;
    }

    _coeffs.b0 = b0;
    _coeffs.b1 = b1;
    _coeffs.b2 = b2;
    _coeffs.a1 = a1;
    _coeffs.a2 = a2;

    return true;
}
