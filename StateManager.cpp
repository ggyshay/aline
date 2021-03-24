#include "StateManager.h"

void StateManager::loadBank(int i, Sequence *s)
{
    Serial.printf("loading bank %d\n", i);
    // Sequence tmp_s;
    EEPROM.get(sizeof(Sequence) * i, *s);
}

void StateManager::saveBank(int i, Sequence *s)
{
    Serial.printf("saving bank %d\n", i);
    Serial.printf("one sequence is %d bytes\n", sizeof(Sequence));
    // Sequence tmp_s;
    EEPROM.put(sizeof(Sequence) * i, *s);
}