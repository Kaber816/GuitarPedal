#include "tremolo_effect.h"
#include "Synthesis/oscillator.h"
#include "guitar_pedal.h"
#include "hid/audio.h"

void TremoloEffect::Init(float freq, float depth, float samplerate)
{
    tremolo.Init(SAMPLE_RATE);
    tremolo.SetWaveform(Oscillator::WAVE_SIN);
    tremolo.SetFreq(freq);
    tremolo.SetDepth(depth);
}

float TremoloEffect::Process(float freq, float depth, float in)
{
    tremolo.SetFreq(freq);
    tremolo.SetDepth(depth);
    return tremolo.Process(in);
}

