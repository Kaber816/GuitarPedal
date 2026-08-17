#include "guitar_pedal.h"
#include <cstdio>

// Effects Includes
#include "effects/tremolo/tremolo_effect.h"
#include "ui/UI.h"
#include "utils/looper/looper_util.h"

// Hardware and Effects variables
static GuitarPedal   pedal;
static TremoloEffect tremolo;
static LooperUtil    looper;

// Other global variables
bool leftEffect;

enum class LooperState { EMPTY, RECORDING, PLAYING };
static LooperState looperState;

static void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    pedal.ProcessAnalogControls();
    pedal.ProcessDigitalControls();

    // Switch state updates, do left first then right so effects stack into looper
    if (pedal.switches[GuitarPedal::SW_5]->FallingEdge())
    {
        leftEffect = !leftEffect;
        pedal.leds[GuitarPedal::LED_L]->Set(leftEffect ? 1.0f : 0.0f);
    }

    if (pedal.switches[GuitarPedal::SW_6]->FallingEdge())
    {
        if (pedal.switches[GuitarPedal::SW_6]->TimeHeldMs() > 1000.0f)
        {
            if (looper.IsRecording())
            {
                looper.TrigRecord();
            }

            looper.ClearBuffer();
            looperState = LooperState::EMPTY;
            pedal.leds[GuitarPedal::LED_R]->Set(0.0f);
        }
        else
        {
            switch (looperState)
            {
                case LooperState::EMPTY:
                    looper.TrigRecord();
                    looperState = LooperState::RECORDING;
                    break;

                case LooperState::RECORDING:
                    looper.TrigRecord();
                    looperState = LooperState::PLAYING;
                    break;

                case LooperState::PLAYING:
                    looper.TrigRecord();
                    looperState = LooperState::RECORDING;
                    break;
            }

            pedal.leds[GuitarPedal::LED_R]->Set(looperState == LooperState::EMPTY ? 0.0f : 1.0f);
        }
    }

    float freq  = 10.0f * pedal.GetKnobValue(GuitarPedal::KNOB_3);
    float depth = pedal.GetKnobValue(GuitarPedal::KNOB_6);

    for (size_t i = 0; i < size; i++)
    {
        float sample = in[0][i];

        if (leftEffect)
            sample = tremolo.Process(freq, depth, sample);

        if (looperState != LooperState::EMPTY)
        {
            float looped = looper.Process(sample);
            sample = (looperState == LooperState::RECORDING) ? (sample + looped) : looped;
        }

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
    looperState = LooperState::EMPTY;
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
