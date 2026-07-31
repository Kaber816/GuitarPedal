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
        knobs[i].SetSampleRate(AudioCallbackRate());
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
    knobs[0].Process();
    knobs[1].Process();
    knobs[2].Process();
    knobs[3].Process();
    knobs[4].Process();
    knobs[5].Process();
}

void ProcessDigitalControls()
{
    
}

float GuitarPedal::GetKnobValue(Knob k)
{
    size_t idx;
    idx = k < KNOB_6 ? k : KNOB_1;
    return knobs[idx].Value();
}


void GuitarPedal::InitKnobs()
{
    // Configure ADC channels for potentiometer pins
    AdcChannelConfig knob_init[KNOB_COUNT]; // ADC configuration object
    knob_init[KNOB_1].InitSingle(KNOB_1_PIN);
    knob_init[KNOB_2].InitSingle(KNOB_2_PIN);
    knob_init[KNOB_3].InitSingle(KNOB_3_PIN);
    knob_init[KNOB_4].InitSingle(KNOB_4_PIN);
    knob_init[KNOB_5].InitSingle(KNOB_5_PIN);
    knob_init[KNOB_6].InitSingle(KNOB_6_PIN);

    seed.adc.Init(knob_init, KNOB_COUNT);

    // Fill the empty array with references to the knew initialized knobs
    knobs[KNOB_1] = &knob1;
    knobs[KNOB_1] = &knob2;
    knobs[KNOB_1] = &knob3;
    knobs[KNOB_1] = &knob4;
    knobs[KNOB_1] = &knob5;
    knobs[KNOB_1] = &knob6;

    for (int i = 0; i < KNOB_COUNT; i++)
    {
        knobs[i]->Init(seed.adc.GetPtr(i), seed.AudioCallbackRate());
    }
}

void GuitarPedal::InitSwitches()
{

}

void GuitarPedal::InitLeds()
{

}
