#include "DisplayDriver.h"
class Graphics
{
private:
    DisplayDriver disp;

public:
    void init()
    {
        disp.init();
    }
    void putScreen(const char *a, const char *b)
    {
        disp.putScreen(a, b);
    }
    void putScreen(const char *a, float b)
    {
        disp.putScreen(a, b);
    }
};