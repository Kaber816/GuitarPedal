#include "Effects/sampleratereducer.h"
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
static daisysp::SampleRateReducer sampleRateReducer;

// Other global variables
bool effectsEnabled;
bool rightLed;
bool loopClearHasToggled;

static void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    pedal.ProcessAnalogControls();
    pedal.ProcessDigitalControls();

    // Left effect/LED handling
    if (pedal.switches[GuitarPedal::SW_5]->RisingEdge())
    {
        effectsEnabled = !effectsEnabled;
        pedal.leds[GuitarPedal::LED_L]->Set(effectsEnabled ? 1.0f : 0.0f);
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

    // Effect settings with knobs
    float tremoloFreq  = 10.0f * pedal.GetKnobValue(GuitarPedal::KNOB_3);
    float tremoloDepth = pedal.GetKnobValue(GuitarPedal::KNOB_6);
    sampleRateReducer.SetFreq(0.1 * pedal.GetKnobValue(GuitarPedal::KNOB_5));

    for (size_t i = 0; i < size; i++)
    {
        // Original input sample, stack effects together to get final output
        float sample = in[0][i];

        // Effects processing
        if (effectsEnabled)
        {
            // Sample Rate Reducer
            if (pedal.switches[GuitarPedal::SW_3]->Pressed())
            {
                sample = sampleRateReducer.Process(sample);
            }

            // Tremolo
            if (pedal.switches[GuitarPedal::SW_4]->Pressed())
            {
                sample = tremolo.Process(tremoloFreq, tremoloDepth, sample);
            }

        }

        float looped = looper.Process(sample);
        sample += looped; // Add the loop ontop so it replays with current signal

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
    effectsEnabled  = false;
    rightLed = false;
    pedal.leds[GuitarPedal::LED_L]->Set(0.0f);
    pedal.leds[GuitarPedal::LED_R]->Set(0.0f);

    // Effects setup
    tremolo.Init(pedal.GetKnobValue(GuitarPedal::KNOB_3), pedal.GetKnobValue(GuitarPedal::KNOB_6), SAMPLE_RATE);

    looper.Init();
    loopClearHasToggled = false;

    sampleRateReducer.Init();
    sampleRateReducer.SetFreq(pedal.GetKnobValue(GuitarPedal::KNOB_5));

    // Enable logging, and set up USB connection
    pedal.seed.StartLog();

    while(1)
    {
    }
}
