// #include "StateManager.h"

// void StateManager::loadBank(int i, Sequence *s)
// {
//     EEPROM.get(sizeof(Sequence) * i, *s);
// }

// void StateManager::saveBank(int i, Sequence *s)
// {
//     EEPROM.put(sizeof(Sequence) * i, *s);
// }
// void StateManager::maskFromBank(int i, Sequence *s)
// {
//     Sequence tmp_s;
//     EEPROM.get(sizeof(Sequence) * i, tmp_s);
//     for (unsigned char i = 0; i < 64; i++)
//     {
//         s->notes[i].velocity = tmp_s.notes[i].velocity;
//     }
// }