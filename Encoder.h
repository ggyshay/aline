#include <stdlib.h>
#include <functional>

class Encoder
{
public:
    bool setReading(bool A, bool B, bool C)
    {
        bool changed = false;
        if (C != lastClickState)
        {
            if (C)
            {
                changed = true;
                onClick();
            }
            lastClickState = C;
        }
        if (!B && lastStateA != A)
        {
            if (A)
            {
                Serial.println("incrementing");
                onIncrement();
            }
            else
            {
                Serial.println("decrement");
                onDecrement();
            }
            // value = A ? value + 1 : value - 1;
            changed = true;
        }

        lastStateA = A;

        return changed;
    }

    std::function<void(void)> onIncrement = nullptr;
    std::function<void(void)> onDecrement = nullptr;
    std::function<void(void)> onClick = nullptr;

    int value = 0;
    bool lastClickState = false;
    bool lastStateA = false;
    bool state = false;
};