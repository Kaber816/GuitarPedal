#pragma once
#ifndef SAMPLE_RATE
#define SAMPLE_RATE 48014.f
#endif

#include "daisy_seed.h"

class GuitarPedal
{
    public:
        /* Knobs enums */
        enum Knob 
        {
            KNOB_1, /* Top left pot */
            KNOB_2, /* Top middle pot */
            KNOB_3, /* Top right pot */
            KNOB_4, /* Bottom left pot */
            KNOB_5, /* Bottom middle pot */
            KNOB_6, /* Bottom right pot */
            KNOB_COUNT, /* Num knobs/truct end */
        };

        /* Switches enums */
        enum Switches
        {
            SW_1, /* Leftmost switch */
            SW_2, /* Left-center switch */
            SW_3, /* Right-center switch */
            SW_4, /* Rightmost switch */
            SW_5, /* Left footswitch */
            SW_6, /* Right footswitch */
            SW_COUNT, /* Num switches/struct end */
        };
        
        /* LED enums */
        enum LED {
            LED_L, /* Left LED */
            LED_R, /* Right LED */
            LED_COUNT, /* Num LEDs/struct end */
        };

        /* Constructor */
        GuitarPedal() {}

        /* Destructor */
        ~GuitarPedal() {}
        
        /* Initalizes the daisy seed, and GuitarPedal hardware.*/
        void Init(bool boost = false);

        /* 
        Wait some ms before going on.
        \param del Delay time in ms.
        */
        void DelayMs(size_t del);

        /* Starts the callback
        \param cb multichannel callback function
        */
        void StartAudio(daisy::AudioHandle::AudioCallback cb);

        /*
           Switch callback functions
           \param cb New multichannel callback function.
        */
        void ChangeAudioCallback(daisy::AudioHandle::AudioCallback cb);

        /* Stops the audio if it is running. */
        void StopAudio();

        /* Updates the Audio Sample Rate, and reinitializes.
         ** Audio must be stopped for this to work.
         */
        void SetAudioSampleRate(daisy::SaiHandle::Config::SampleRate samplerate);

        /* Returns the audio sample rate in Hz as a floating point number.
         */
        float AudioSampleRate();

        /* Sets the number of samples processed per channel by the audio callback.
           \param size Audio block size
         */
        void SetAudioBlockSize(size_t size);

        /* Returns the number of samples per channel in a block of audio. */
        size_t AudioBlockSize();

        /* Returns the rate in Hz that the Audio callback is called */
        float AudioCallbackRate();

        /* Start analog to digital conversion. */
        void StartAdc();

        /* Stops Transfering data from the ADC */
        void StopAdc();

        /* Call at the same frequency as controls are read for stable readings.*/
        void ProcessAnalogControls();

        /* Call at the same frequency as controls are read for stable readings.*/
        void ProcessDigitalControls();
        
        /* Get value per knob.
        \param k Which knob to get
        \return Floating point knob position.
        */
        float GetKnobValue(Knob k);

        /*
           Set footswitch LED
           \param idx Led Index
           \param bright Brightness
         */
        void SetLed(LED idx, float bright);

        /* Update Leds.*/
        void UpdateLeds();

        daisy::DaisySeed     seed;    /*< & */
        daisy::Encoder       encoder; /*< & */
        daisy::AnalogControl knob1, knob2, knob3, knob4, knob5, knob6, *knobs[KNOB_COUNT]; /*< & */
        daisy::Switch        switch1, switch2, switch3, switch4, switch5, switch6, *switches[SW_COUNT]; /*< & */
        daisy::Led           ledLeft, ledRight, *leds[2]; /*< & */

      private:
        void SetHidUpdateRates();
        void InitSwitches();
        void InitKnobs();
        void InitLeds();

        inline uint16_t* adc_ptr(const uint8_t chn) { return seed.adc.GetPtr(chn); }

};
