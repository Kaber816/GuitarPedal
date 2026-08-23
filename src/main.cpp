#include "Utility/looper.h"
#include "guitar_pedal.h"
#include <cstdio>

// Effects Includes
#include "effects/tremolo/tremolo_effect.h"
#include "daisysp.h"
#include "utils/looper/looper_util.h"

// Hardware and Effects variables
static GuitarPedal   pedal;
static TremoloEffect tremolo;
static LooperUtil looper;

// Other global variables
bool leftEffect;
bool rightLed;
bool loopClearHasToggled;

static void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    pedal.ProcessAnalogControls();
    pedal.ProcessDigitalControls();

    // Left effect/LED handling
    if (pedal.switches[GuitarPedal::SW_5]->RisingEdge())
    {
        leftEffect = !leftEffect;
        pedal.leds[GuitarPedal::LED_L]->Set(leftEffect ? 1.0f : 0.0f);
    }
    
    // Clear Looper Handling
    if (pedal.switches[GuitarPedal::SW_6]->Pressed())
    {
        if (pedal.switches[GuitarPedal::SW_6]->TimeHeldMs() > 1000.0f && !loopClearHasToggled) 
        {
            rightLed = !rightLed;
            pedal.leds[GuitarPedal::LED_R]->Set(0.0f);
            loopClearHasToggled = true;
            looper.Clear();
        }
    }
    else
    {
        loopClearHasToggled = false;
    }

    if (pedal.switches[GuitarPedal::SW_6]->RisingEdge() && !loopClearHasToggled)
    {
        looper.TrigRecord();
        if (looper.Recording())
        {
            pedal.leds[GuitarPedal::LED_R]->Set(1.0f);
        }
    }

    float freq  = 10.0f * pedal.GetKnobValue(GuitarPedal::KNOB_3);
    float depth = pedal.GetKnobValue(GuitarPedal::KNOB_6);

    for (size_t i = 0; i < size; i++)
    {
        float sample = in[0][i];

        if (leftEffect)
        {
            sample = tremolo.Process(freq, depth, sample);
        }

        float looped = looper.Process(sample);
        sample += looped;

        out[0][i] = sample;
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
    leftEffect  = false;
    rightLed = false;
    loopClearHasToggled = false;
    pedal.leds[GuitarPedal::LED_L]->Set(0.0f);
    pedal.leds[GuitarPedal::LED_R]->Set(0.0f);

    // Effects setup
    tremolo.Init(pedal.GetKnobValue(GuitarPedal::KNOB_3), pedal.GetKnobValue(GuitarPedal::KNOB_6), SAMPLE_RATE);
    looper.Init();

    // Enable logging, and set up USB connection
    pedal.seed.StartLog();

    while(1)
    {
        pedal.seed.PrintLine("Freq: %d, Depth: %d", static_cast<int>(pedal.GetKnobValue(GuitarPedal::KNOB_3)), static_cast<int>(pedal.GetKnobValue(GuitarPedal::KNOB_6)));
    }
}
