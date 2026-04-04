#include "TimerClass.hpp"

TimerClass::TimerClass() {}

TimerClass::TimerClass(const TimerClass& other) {}

TimerClass::~TimerClass() {}

bool TimerClass::Initialize() {
    INT64 frequency;


    QueryPerformanceFrequency(reinterpret_cast <LARGE_INTEGER*> (&frequency));
    if (frequency == 0) {
        return false;
    }

    m_frequency = static_cast <float> (frequency);

    QueryPerformanceCounter(reinterpret_cast <LARGE_INTEGER*> (&m_startTime));

    return true;
}

void TimerClass::Frame() {
    INT64 currentTime;
    INT64 elapsedTicks;


    QueryPerformanceCounter(reinterpret_cast <LARGE_INTEGER*> (&currentTime));
    elapsedTicks = currentTime - m_startTime;

    m_frameTime = static_cast<float>(elapsedTicks) / m_frequency;

    m_startTime = currentTime;

    return;
}

float TimerClass::GetTime() {
    return m_frameTime;
}

