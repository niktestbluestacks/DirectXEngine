#pragma once
#ifndef _TIMERCLASS_HPP_
#define _TIMERCLASS_HPP_

// Includes
#include <windows.h>

// Class name: TimerClass
class TimerClass {
public:
    TimerClass();
    TimerClass(const TimerClass&);
    ~TimerClass();

    bool Initialize();
    void Frame();

    float GetTime();

private:
    float m_frequency;
    INT64 m_startTime;
    float m_frameTime;
};
#endif // _TIMERCLASS_HPP_
