#include "guitar_pedal.h"
#include <cstdio>

// Effects Includes
#include "effects/tremolo/tremolo_effect.h"

namespace
{
// Hardware and Effects variables
static GuitarPedal pedal;
static TremoloEffect tremolo;
}

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
            out[0][i] = tremolo.Process((10.0f * pedal.GetKnobValue(GuitarPedal::KNOB_3))
                    ,pedal.GetKnobValue(GuitarPedal::KNOB_6), in[0][i]);
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
    tremolo.Init(pedal.GetKnobValue(GuitarPedal::KNOB_3), pedal.GetKnobValue(GuitarPedal::KNOB_6),SAMPLE_RATE); 

    // Enable logging, and set up USB connection
    pedal.seed.StartLog();


    while(1)
    {
        pedal.seed.PrintLine("Freq: %d, Depth: %d", static_cast<int>(pedal.GetKnobValue(GuitarPedal::KNOB_3)), static_cast<int>(pedal.GetKnobValue(GuitarPedal::KNOB_6)));
    }
}
