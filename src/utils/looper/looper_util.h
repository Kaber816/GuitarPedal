#pragma once
#include "daisysp.h"

class LooperUtil
{
    public:
    LooperUtil(){}   
    ~LooperUtil(){}   
    void Init();
    float Process(float in);
    bool IsRecording();
    void ClearBuffer();
    void TrigRecord();


    private:
    daisysp::Looper looper;
};
