#ifndef MIDIEVENT_H
#define MIDIEVENT_H
class MIDIEvent
{
public:
    void set(int _channel, int _note, int _velocity, int _duration);
    int channel;
    int note;
    int velocity;
    int duration;
};

class EdgeEvent
{
public:
    EdgeEvent(unsigned long timestamp, int _channel, int _note, int _velocity);
    int channel;
    int note;
    int velocity;
    unsigned long timestamp;
};
#endif
