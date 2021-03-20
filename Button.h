#define debounceDelay 200
class Button
{
public:
    bool *value;
    bool isReleaseSensitive;
    void setReading(bool);
    Button(bool *_value, bool _isReleaseSensitive);
    void setPointer(bool *ptr);

private:
    bool buttonState = false;
    // bool lastButtonState = false;
    unsigned long lastDebounceTime = 0;
};

void Button::setReading(bool reading)
{
    // if (reading != lastButtonState)
    // {
    //     lastDebounceTime = millis();
    // }

    if (((millis() - lastDebounceTime) > debounceDelay) && reading != buttonState)
    {
        // buttonState = reading;

        // if (isReleaseSensitive)
        // {
        buttonState = reading;
        *value = buttonState;
        lastDebounceTime = millis();
        // }
        // else
        // // {
        //     if (buttonState == HIGH)
        //     {
        //         *value = !*value;
        //     }
        // }
    }
}

Button::Button(bool *_value, bool _isReleaseSensitive)
{
    value = _value;
    isReleaseSensitive = _isReleaseSensitive;
}

void Button::setPointer(bool *ptr)
{
    value = ptr;
}