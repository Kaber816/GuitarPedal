#pragma once

#include "daisy_seed.h"

namespace daisy
{
/**Helpers and hardware definitions for the GuitarPedal.*/
class GuitarPedal
{
    public:
        /** Knobs */
        enum Knob 
        {
            KNOB_1, /** Top left pot */
            KNOB_2, /** Top middle pot */
            KNOB_3, /** Top right pot */
            KNOB_4, /** Bottom left pot */
            KNOB_5, /** Bottom middle pot */
            KNOB_6, /** Bottom righr pot */
        };

        /** Switches */
        enum Switches
        {
            SWITCH_1, /** Leftmost switch */
            SWITCH_2, /** Left-center switch */
            SWITCH_3, /** Right-center switch */
            SWITCH_4, /** Rightmost switch */
        };

        /** Switches */
        enum LED {
            LED_L, /** Left LED */
            LED_R, /** Right LED */
        };

        /** Constructor */
        GuitarPedal() {}

        /** Destructor */
        ~GuitarPedal() {}
        
        /** Initalizes the daisy seed, and GuitarPedal hardware.*/
        void Init(bool boost = false);

        /** 
        Wait some ms before going on.
        \param del Delay time in ms.
        */
        void DelayMs(size_t del);

        /** Starts the callback
        \param cb multichannel callback function
        */
        void StartAudio(AudioHandle::AudioCallback cb);

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
        void SetLed(Led idx, float bright);

        DaisySeed     seed;    /**< & */
        Encoder       encoder; /**< & */
        AnalogControl knobs[6]; /**< & */
        Switch        switches[4] /**< & */;
        Led           leds[2]; /**< & */

      private:
        void SetHidUpdateRates();
        void InitSwitches();
        void InitKnobs();
        void InitLeds();

        inline uint16_t* adc_ptr(const uint8_t chn) { return seed.adc.GetPtr(chn); }

        LedDriverPca9685<2, true> led_driver_;
};

}
