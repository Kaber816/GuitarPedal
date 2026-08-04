#include "Synthesis/oscillator.h"
#include "guitar_pedal.h"
#include "daisysp.h"
#include <cstdio>

using namespace daisy;
using namespace daisysp;

// Pedal hardware
static daisy::GuitarPedal pedal;

// Effects
Tremolo tremolo;

// Global variables
bool bypass;

static void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    pedal.ProcessAnalogControls();
    pedal.ProcessDigitalControls();
    
    // Switch state updates
    if (pedal.switches[GuitarPedal::SW_5]->RisingEdge())
    {
        bypass = !bypass;
        pedal.leds[GuitarPedal::LED_L]->Set(bypass ? 1.0f : 0.0f);
    }

    // TREMOLO
    tremolo.SetFreq(pedal.GetKnobValue(daisy::GuitarPedal::KNOB_1));
    tremolo.SetDepth(pedal.GetKnobValue(daisy::GuitarPedal::KNOB_2));

    if (!bypass)
    {
        for (size_t i = 0; i < size; i++)
        {
            out[0][i] = in[0][i];
        }
    }
    else
    {
        for (size_t i = 0; i < size; i++)
        {
            out[0][i] = tremolo.Process(in[0][i]);
        }
    }

    pedal.UpdateLeds();
}

int main(void)
{
    // Initialize the hardware
    pedal.Init(false);
    pedal.StartAdc();
    pedal.StartAudio(AudioCallback);

    // Pedal state setup
    bypass = true;
    
    // Effects setup
    tremolo.Init(SAMPLE_RATE);
    tremolo.SetWaveform(Oscillator::WAVE_SIN);
    tremolo.SetFreq(pedal.GetKnobValue(daisy::GuitarPedal::KNOB_1));
    tremolo.SetDepth(pedal.GetKnobValue(daisy::GuitarPedal::KNOB_2));
    

    // Enable logging, and set up USB connection
    pedal.seed.StartLog();


    while(1)
    {
        pedal.seed.PrintLine("Bypass: %d", bypass);
    }
}
