#include <stdlib.h>
#include <functional>
#include "Arduino.h"
#define debounceDelay 200
class Button
{
public:
    bool *value;
    bool isReleaseSensitive;
    Button(bool *_value, bool _isReleaseSensitive)
    {
        value = _value;
        isReleaseSensitive = _isReleaseSensitive;
    }
    void setPointer(bool *ptr)
    {
        value = ptr;
    }
    std::function<void(void)> onToggle = nullptr;

    void setReading(bool reading)
    {

        if (((millis() - lastDebounceTime) > debounceDelay) && reading != buttonState)
        {
            if (isReleaseSensitive)
            {
                *value = reading;
                buttonState = reading;
                lastDebounceTime = millis();
            }
            else
            {
                if (reading == HIGH)
                {
                    *value = !*value;
                    if (onToggle != nullptr)
                    {
                        onToggle();
                    }
                }
                buttonState = reading;
                lastDebounceTime = millis();
            }
        }
    }

private:
    bool buttonState = false;
    unsigned long lastDebounceTime = 0;
};