#include <stdlib.h>
#include <functional>
#include <algorithm>
#include <vector>
#include "midiEvent.h"
#include "arduino.h"
#include "Constants.h"

class OutputBuffer
{
private:
    std::vector<EdgeEvent> buffer;
    std::function<void(EdgeEvent e)> sendCallback;

public:
    int registerEvent(MIDIEvent *(event));
    void registerClock();
    void setOutputCallback(std::function<void(EdgeEvent e)> callback);
    unsigned long sendNext();
    EdgeEvent *getNextEdge();
};
