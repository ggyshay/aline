#include "scheduler.h"

static void handleClockOuter();
Scheduler::Scheduler(/* args */)
{
}

void Scheduler::begin()
{
    internalClockTimer.begin(handleClockOuter, 15000000 / bpmEstimate);
}

void Scheduler::tick()
{
    if (tickCallback != nullptr)
    {
        tickCallback();
    }
}

void Scheduler::setTickCallback(std::function<void(void)> callback)
{
    tickCallback = callback;
}

void Scheduler::onMIDIClock()
{
    unsigned long now = micros();
    if (lastClock != 0)
    {
        int pulseDelta = now - lastClock;
        bpmEstimate = bpmEstimate * 0.85 + 0.15 * (60000000 / (pulseDelta * 4));
    }
    lastClock = now;
}

void Scheduler::setOnSendCallback(std::function<void(void)> callback)
{
    onSendCallback = callback;
}

void Scheduler::registerTimeToNextEvent(unsigned long timeToNextEventMicros)
{
    if (timeToNextEventMicros == MAX_U_LONG_VAL)
    {
        internalClockTimer.end();
    }
    else
    {
        Serial.printf("unlocking and setting next timeout of %dms\n", timeToNextEventMicros);
        internalClockTimer.begin(handleClockOuter, timeToNextEventMicros);
    }
}

static void handleClockOuter()
{
    scheduler.tick();
}