#pragma once
#include "daisysp.h"

class LooperUtil
{
    public:
    LooperUtil(){}   
    ~LooperUtil(){}   
    void Init();
    float Process(float in);
    bool Recording();
    void Clear();
    void TrigRecord();
    void ChangeMode();


    private:
    daisysp::Looper looper;
};
