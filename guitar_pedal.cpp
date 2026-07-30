#include "guitar_pedal.h"

using namespace daisy;

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
    knob1.Process();
}

float GuitarPedal::GetKnobValue(Knob k)
{
    size_t idx;
    idx = k < KNOB_LAST ? k : KNOB_1;
    return knobs[idx]->Value();
}

void GuitarPedal::ProcessDigitalControls()
{
    switch1.Debounce();
}

void GuitarPedal::ClearLeds()
{
    // Using Color
    Color c;
    c.Init(Color::PresetColor::OFF);
    led1.SetColor(c);
    led2.SetColor(c);
    // Without
    // led1.Set(0.0f, 0.0f, 0.0f);
    // led2.Set(0.0f, 0.0f, 0.0f);
}

void GuitarPedal::UpdateLeds()
{
    led1.Update();
    led2.Update();
}

void GuitarPedal::InitButtons()
{
    // button1
    button1.Init(SW_1_PIN);
    // button2
    button2.Init(SW_2_PIN);

    buttons[BUTTON_1] = &button1;
    buttons[BUTTON_2] = &button2;
}

void GuitarPedal::InitEncoder()
{
    encoder.Init(ENC_A_PIN, ENC_B_PIN, ENC_CLICK_PIN);
}

void GuitarPedal::InitLeds()
{
    // LEDs are just going to be on/off for now.
    // TODO: Add PWM support
    led1.Init(LED_1_R_PIN, LED_1_G_PIN, LED_1_B_PIN, true);

    led2.Init(LED_2_R_PIN, LED_2_G_PIN, LED_2_B_PIN, true);

    ClearLeds();
    UpdateLeds();
}
void GuitarPedal::InitKnobs()
{
    // Configure the ADC channels using the desired pin
    AdcChannelConfig knob_init[KNOB_LAST];
    knob_init[KNOB_1].InitSingle(KNOB_1_PIN);
    knob_init[KNOB_2].InitSingle(KNOB_2_PIN);
    // Initialize with the knob init struct w/ 2 members
    // Set Oversampling to 32x
    seed.adc.Init(knob_init, KNOB_LAST);
    // Make an array of pointers to the knobs.
    knobs[KNOB_1] = &knob1;
    knobs[KNOB_2] = &knob2;
    for(int i = 0; i < KNOB_LAST; i++)
    {
        knobs[i]->Init(seed.adc.GetPtr(i), seed.AudioCallbackRate());
    }
}
void GuitarPedal::InitMidi()
{
    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
}

