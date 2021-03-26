#include "Sequence.h"
#include <EEPROM.h>

class StateManager
{
private:
    Sequence *sequence;

public:
    void loadBank(int i, Sequence *s);
    void saveBank(int i, Sequence *s);
    void maskFromBank(int i, Sequence *s);
};