#include "midiEvent.h"

void MIDIEvent::set(int _channel, int _note, int _velocity, int _duration)
{
    channel = _channel;
    note = _note;
    velocity = _velocity;
    duration = _duration;
}

EdgeEvent::EdgeEvent(unsigned long _timestamp, int _channel, int _note, int _velocity)
{
    timestamp = _timestamp;
    channel = _channel;
    note = _note;
    velocity = _velocity;
}