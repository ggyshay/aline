#define S0 9
#define S1 10
#define S2 11
#define S3 12
#define LED_PIN 24
#define BUTTONS_PIN 25
#define AUXBUTTONS_PIN 26
#define ENCODERS_A_PIN 28
#define ENCODERS_B_PIN 29
#define ENCODERS_C_PIN 27

namespace std
{
    void __throw_bad_alloc()
    {
        Serial.println("Unable to allocate memory");
        while (1)
            ;
    }

    void __throw_length_error(char const *e)
    {
        Serial.print("Length Error :");
        Serial.println(e);
        while (1)
            ;
    }

    void __throw_bad_function_call()
    {
        Serial.println("Bad function call!");
        while (1)
            ;
    }
    template class basic_string<char>;
    void __throw_logic_error(char const *err)
    {
        Serial.println(err);
        while (true)
            ;
    }
} // namespace std

void setupPins()
{
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTONS_PIN, INPUT);
    pinMode(AUXBUTTONS_PIN, INPUT);
    pinMode(ENCODERS_A_PIN, INPUT);
    pinMode(ENCODERS_B_PIN, INPUT);
    pinMode(ENCODERS_C_PIN, INPUT);
}

void sendBits(char i)
{
    digitalWriteFast(S0, i & 1);
    digitalWriteFast(S1, i & 2);
    digitalWriteFast(S2, i & 4);
    digitalWriteFast(S3, i & 8);
    delayMicroseconds(100);
}

bool getButtons()
{
    return digitalReadFast(BUTTONS_PIN);
}

bool getAuxButtons()
{
    return digitalReadFast(AUXBUTTONS_PIN);
}

bool getEncodersA()
{
    return digitalReadFast(ENCODERS_A_PIN);
}

bool getEncodersB()
{
    return digitalReadFast(ENCODERS_B_PIN);
}
bool getEncodersC()
{
    return digitalReadFast(ENCODERS_C_PIN);
}

void writeLed(bool value)
{
    digitalWriteFast(LED_PIN, value);
}