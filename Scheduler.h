#include <stdlib.h>
#include <functional>
#include "IntervalTimer.h"
#include "arduino.h"
#include "Constants.h"

class Scheduler
{
private:
    IntervalTimer internalClockTimer;
    unsigned long lastClock = 0;
    std::function<void(void)> tickCallback;
    std::function<void(void)> onSendCallback;

public:
    Scheduler();
    void tick();
    void begin();
    void setTickCallback(std::function<void(void)> callback);
    void registerTimeToNextEvent(unsigned long timeToNextEventMicros);
    void onMIDIClock(void);
    void setOnSendCallback(std::function<void(void)> callback);
    float bpmEstimate = 120;
};

extern Scheduler scheduler;