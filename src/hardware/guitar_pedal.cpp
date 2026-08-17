#include "guitar_pedal.h"

using namespace daisy;

// Pins
constexpr Pin SW_1_PIN = seed::D10; /* Leftmost switch*/
constexpr Pin SW_2_PIN = seed::D9; /* Center left switch*/
constexpr Pin SW_3_PIN = seed::D8; /* Center right switch*/
constexpr Pin SW_4_PIN = seed::D7; /* Rightmost switch*/
constexpr Pin SW_5_PIN = seed::D25; /* Left  footswitch*/
constexpr Pin SW_6_PIN = seed::D26; /* Right footswitch*/

constexpr Pin KNOB_1_PIN = seed::A1; /* Top left pot */
constexpr Pin KNOB_2_PIN = seed::A2; /* Top middle pot */
constexpr Pin KNOB_3_PIN = seed::A3; /* Top right pot */
constexpr Pin KNOB_4_PIN = seed::A4; /* Bottom left pot */
constexpr Pin KNOB_5_PIN = seed::A5; /* Bottom middle pot */
constexpr Pin KNOB_6_PIN = seed::A6; /* Bottom right pot */

constexpr Pin LED_L_PIN = seed::D22; /* Left LED */
constexpr Pin LED_R_PIN = seed::D23; /* Right LED */

/*
 *Initializes the GuitarPedal hardware.
 *boost: Overclocks the processor speed
 */
void GuitarPedal::Init(bool boost)
{
    // Set Some numbers up for accessors.
    // Initialize the hardware.
    seed.Configure();
    seed.Init(boost);
    SetAudioBlockSize(48);
    InitLeds();
    InitKnobs();
    InitSwitches();
}

void GuitarPedal::DelayMs(size_t del)
{
    seed.DelayMs(del);
}


void GuitarPedal::SetHidUpdateRates()
{
    for(int i = 0; i < KNOB_COUNT; i++)
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

float GuitarPedal::GetKnobValue(Knob idx)
{
    idx = idx < KNOB_COUNT ? idx : KNOB_1;
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
    // DPDT switches setup
    constexpr Pin pin_numbers[SW_COUNT] =
    {
        SW_1_PIN,
        SW_2_PIN,
        SW_3_PIN,
        SW_4_PIN,
        SW_5_PIN,
        SW_6_PIN,
    };

    switches[SW_1] = &switch1;
    switches[SW_2] = &switch2;
    switches[SW_3] = &switch3;
    switches[SW_4] = &switch4;
    switches[SW_5] = &switch5;
    switches[SW_6] = &switch6;


    for (size_t i = 0; i < SW_COUNT; i++)
    {
        switches[i]->Init(pin_numbers[i]);
    }
}

void GuitarPedal::InitLeds()
{
    leds[LED_L] = &ledLeft;
    leds[LED_R] = &ledRight;
    
    // Initialize LEDS, sample rate defaults to 1000 Hz
    leds[LED_L]->Init(LED_L_PIN, false);
    leds[LED_R]->Init(LED_R_PIN, false);

    for (size_t i = 0; i < LED_COUNT; i++)
    {
        leds[i]->Set(0.0f);
        leds[i]->Update();
    }
}

void GuitarPedal::SetLed(LED idx, float bright) {
    leds[idx]->Set(bright);
}

/* Update Leds.*/
void GuitarPedal::UpdateLeds()
{
    for (size_t i = 0; i < LED_COUNT; i++)
    {
        leds[i]->Update();
    }
}
