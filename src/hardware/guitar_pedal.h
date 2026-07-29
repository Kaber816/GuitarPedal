#pragma once

#include "daisy_seed.h"
#include "terrarium.h"

namespace daisy
{
//Helpers and hardware definitions for the GuitarPedal.
class GuitarPedal
{
    public:
        GuitarPedal() {}
        ~GuitarPedal() {}
        
        void Init();

        /** Starts the callback
        \param cb Interleaved callback function
        */
        void StartAudio(AudioHandle::InterleavingAudioCallback cb);

        /** Starts the callback
        \param cb multichannel callback function
        */
        void StartAudio(AudioHandle::AudioCallback cb);

        /**
           Switch callback functions
           \param cb New interleaved callback function.
        */
        void ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb);

        /**
           Switch callback functions
           \param cb New multichannel callback function.
        */
        void ChangeAudioCallback(AudioHandle::AudioCallback cb);

        /** Stops the audio if it is running. */
        void StopAudio();

        /** Updates the Audio Sample Rate, and reinitializes.
         ** Audio must be stopped for this to work.
         */
        void SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate);

        /** Returns the audio sample rate in Hz as a floating point number.
         */
        float AudioSampleRate();

        /** Sets the number of samples processed per channel by the audio callback.
           \param size Audio block size
         */
        void SetAudioBlockSize(size_t size);

        /** Returns the number of samples per channel in a block of audio. */
        size_t AudioBlockSize();

        /** Returns the rate in Hz that the Audio callback is called */
        float AudioCallbackRate();

        /** Start analog to digital conversion. */
        void StartAdc();

        /** Stops Transfering data from the ADC */
        void StopAdc();

        /** Call at the same frequency as controls are read for stable readings.*/
        void ProcessAnalogControls();
        
        /** Get value per knob.
        \param k Which knob to get
        \return Floating point knob position.
        */
        float GetKnobValue(Knob k);

        /**
           Set footswitch LED
           \param idx Led Index
           \param bright Brightness
         */
        void SetFootswitchLed(FootswitchLed idx, float bright);

        DaisySeed seed;    /**< & */
        Encoder   encoder; /**< & */

        AnalogControl knob[KNOB_LAST]; /**< & */
        AnalogControl expression;      /**< & */
        Switch        switches[SW_LAST] /**< & */;

        RgbLed ring_led[8];       /**< & */
        Led    footswitch_led[4]; /**< & */

      private:
        void SetHidUpdateRates();
        void InitSwitches();
        void InitEncoder();
        void InitLeds();
        void InitAnalogControls();

        inline uint16_t* adc_ptr(const uint8_t chn) { return seed.adc.GetPtr(chn); }

        LedDriverPca9685<2, true> led_driver_;
};

}
