#pragma once

#include "daisysp.h"
#include "hid/audio.h"

using namespace daisysp;
using namespace daisy;

class TremoloEffect
{
    public:
    TremoloEffect() {}
    ~TremoloEffect() {}
    void Init(float freq, float depth, float samplerate);
    float Process(float freq, float depth, float in);

    private:
    Tremolo tremolo;
};
