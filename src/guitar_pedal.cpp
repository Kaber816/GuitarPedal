#include "guitar_pedal.h"

using namespace daisy;

// Pins
constexpr Pin SW_1_PIN = seed::D10;
constexpr Pin SW_2_PIN = seed::D9;
constexpr Pin SW_3_PIN = seed::D8;
constexpr Pin SW_4_PIN = seed::D7;

constexpr Pin KNOB_1_PIN = seed::A1;
constexpr Pin KNOB_2_PIN = seed::A2;
constexpr Pin KNOB_3_PIN = seed::A3;
constexpr Pin KNOB_4_PIN = seed::A4;
constexpr Pin KNOB_5_PIN = seed::A5;
constexpr Pin KNOB_6_PIN = seed::A6;

constexpr Pin LED_L_PIN = seed::D29;
constexpr Pin LED_R_PIN = seed::D30;

/**
 *Initializes the GuitarPedal hardware.
 *boost: Overclocks the processor speed
 */
void GuitarPedal::Init(bool boost)
{
    // Set Some numbers up for accessors.
    // Initialize the hardware.
    seed.Configure();
    seed.Init(boost);
    InitLeds();
    InitKnobs();
    SetAudioBlockSize(48);
}

void GuitarPedal::DelayMs(size_t del)
{
    seed.DelayMs(del);
}


void GuitarPedal::SetHidUpdateRates()
{
    for(int i = 0; i < KNOB_6; i++)
    {
        knobs[i]->SetSampleRate(AudioCallbackRate());
    }
}

void GuitarPedal::StartAudio(AudioHandle::AudioCallback cb)
{
    seed.StartAudio(cb);
}

void GuitarPedal::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void GuitarPedal::StopAudio()
{
    seed.StopAudio();
}

void GuitarPedal::SetAudioBlockSize(size_t size)
{
    seed.SetAudioBlockSize(size);
    SetHidUpdateRates();
}

size_t GuitarPedal::AudioBlockSize()
{
    return seed.AudioBlockSize();
}

void GuitarPedal::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    seed.SetAudioSampleRate(samplerate);
    SetHidUpdateRates();
}

float GuitarPedal::AudioSampleRate()
{
    return seed.AudioSampleRate();
}

float GuitarPedal::AudioCallbackRate()
{
    return seed.AudioCallbackRate();
}

void GuitarPedal::StartAdc()
{
    seed.adc.Start();
}

void GuitarPedal::StopAdc()
{
    seed.adc.Stop();
}

void GuitarPedal::ProcessAnalogControls()
{
    knobs[0]->Process();
    knobs[1]->Process();
    knobs[2]->Process();
    knobs[3]->Process();
    knobs[4]->Process();
    knobs[5]->Process();
}

void GuitarPedal::ProcessDigitalControls()
{
    for (size_t i = 0; i < SW_COUNT; i++)
    {
        switches[i]->Debounce();
    }
}

float GuitarPedal::GetKnobValue(Knob k)
{
    size_t idx;
    idx = k < KNOB_6 ? k : KNOB_1;
    return knobs[idx]->Value();
}


void GuitarPedal::InitKnobs()
{
    // Configure ADC channels for potentiometer pins
    AdcChannelConfig config[KNOB_COUNT]; // ADC configuration object
    config[KNOB_1].InitSingle(KNOB_1_PIN);
    config[KNOB_2].InitSingle(KNOB_2_PIN);
    config[KNOB_3].InitSingle(KNOB_3_PIN);
    config[KNOB_4].InitSingle(KNOB_4_PIN);
    config[KNOB_5].InitSingle(KNOB_5_PIN);
    config[KNOB_6].InitSingle(KNOB_6_PIN);

    seed.adc.Init(config, KNOB_COUNT);

    // Fill the empty array with references to the new initialized knobs
    knobs[KNOB_1] = &knob1;
    knobs[KNOB_2] = &knob2;
    knobs[KNOB_3] = &knob3;
    knobs[KNOB_4] = &knob4;
    knobs[KNOB_5] = &knob5;
    knobs[KNOB_6] = &knob6;

    for (int i = 0; i < KNOB_COUNT; i++)
    {
        knobs[i]->Init(seed.adc.GetPtr(i), seed.AudioCallbackRate());
    }
}

void GuitarPedal::InitSwitches()
{
    constexpr Pin pin_numbers[SW_COUNT] =
    {
        SW_1_PIN,
        SW_2_PIN,
        SW_3_PIN,
        SW_4_PIN,
    };

    switches[SW_1] = &switch1;
    switches[SW_2] = &switch2;
    switches[SW_3] = &switch3;
    switches[SW_4] = &switch4;

    for (size_t i = 0; i < SW_COUNT; i++)
    {
        switches[i]->Init(pin_numbers[i]);
    }
}

void GuitarPedal::InitLeds()
{
    leds[LED_L] = &ledLeft;
    leds[LED_R] = &ledRight;

    leds[LED_L]->Init(LED_L_PIN, true);
    leds[LED_R]->Init(LED_R_PIN, true);

    for (size_t i = 0; i < SW_COUNT; i++)
    {
        leds[i]->SetSampleRate(AudioCallbackRate());
        leds[i]->Set(0);
        leds[i]->Update();
    }
}
