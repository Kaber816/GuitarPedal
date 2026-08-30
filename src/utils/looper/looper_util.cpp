#include "looper_util.h"
#include "Utility/looper.h"
#include "daisy_seed.h"

// 48000 (samples per second) * (60 seconds) * (5 minutes) = 5 minutes of floats
#define LOOP_SIZE (48000 * 60 * 5) 

namespace
{
// DSY_SDRAM_BSS is a macro that places buf in SDRAM (external ram chip off of stm32, slower access but bigger)
float DSY_SDRAM_BSS buf[LOOP_SIZE];
}
                                  
void LooperUtil::Init()
{
    looper.Init(buf, LOOP_SIZE); 
    looper.SetMode(daisysp::Looper::Mode::NORMAL);
}

float LooperUtil::Process(float in)
{
    return  looper.Process(in);
}

bool LooperUtil::Recording()
{
    return looper.Recording();
}

void LooperUtil::Clear()
{
    looper.Clear();
}

void LooperUtil::TrigRecord()
{
    looper.TrigRecord();
}

void LooperUtil::ChangeMode()
{
    switch (looper.GetMode())
    {
        case daisysp::Looper::Mode::NORMAL:
            looper.SetMode(daisysp::Looper::Mode::FRIPPERTRONICS);
            break;
        case daisysp::Looper::Mode::FRIPPERTRONICS:
            looper.SetMode(daisysp::Looper::Mode::NORMAL);
            break;
        default:
            looper.SetMode(daisysp::Looper::Mode::NORMAL);
            break;
    }
}
