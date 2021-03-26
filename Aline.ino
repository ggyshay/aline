#include <Arduino.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string>
#include "HAL.h"
#include "Note.h"
#include "Sequence.h"
#include "Interface.h"
#include "Scheduler.h"
#include "StateManager.h"

namespace std
{
  void __throw_bad_alloc();

  void __throw_length_error(char const *e);

  void __throw_bad_function_call();
  template class basic_string<char>;
  void __throw_logic_error(char const *err);
} // namespace std

Interface interface;
Sequence sequence;
Scheduler scheduler;
StateManager stateManager;

int clockCount = 0;
void handleClock()
{
  clockCount = (clockCount + 1) % 6;
  // Serial.println("clokc");
  if (clockCount == 0)
    scheduler.onClock();
}
void handleStart()
{
  scheduler.onStart();
  scheduler.resetPosition();
  clockCount = 0;
}
void setup()
{
  Serial.begin(112500);
  // while (!Serial)
  //   ;
  Serial.println("Serial Started");
  interface.onSelectionChange = [](char start, char end) -> void {
    sequence.setSelection(start, end);
  };
  interface.onPitchUp = []() -> void { sequence.selectionPitchUp(); };
  interface.onPitchDown = []() -> void { sequence.selectionPitchDown(); };
  interface.onOctUp = []() -> void { sequence.selectionOctUp(); };
  interface.onOctDown = []() -> void { sequence.selectionOctDown(); };
  interface.onCopy = []() -> void { sequence.copySelection(); };
  interface.onPaste = []() -> void { sequence.pasteToSelection(); };
  interface.onDurationUp = []() -> void { sequence.setSelectionDurationUp(); };
  interface.onDurationDown = []() -> void { sequence.setSelectionDurationDown(); };
  interface.onVelocityUp = []() -> void { sequence.setSelectionVelocityUp(); };
  interface.onVelocityDown = []() -> void { sequence.setSelectionVelocityDown(); };
  interface.onLengthUp = []() -> void { sequence.setSequenceLengthUp(); };
  interface.onLengthDown = []() -> void { sequence.setSequenceLengthDown(); };
  interface.onErase = []() -> void { sequence.eraseSequence(); };
  interface.onGateChange = [](bool *gates, char currentPage) -> void { sequence.changeGates(gates, currentPage); };
  interface.onEase = []() -> void { sequence.easeSelection(); };
  interface.onSave = [](int i) -> void { stateManager.saveBank(i, &sequence); };
  interface.onLoad = [](int i) -> void { stateManager.loadBank(i, &sequence); };
  interface.onMask = [](int i) -> void { stateManager.maskFromBank(i, &sequence); };
  interface.setRootUp = [](void) -> void { sequence.setRootUp(); };
  interface.setRootDown = [](void) -> void { sequence.setRootDown(); };
  interface.setScaleUp = [](void) -> void { sequence.setScaleUp(); };
  interface.setScaleDown = [](void) -> void { sequence.setScaleDown(); };
  interface.setSeedUp = [](void) -> void { sequence.setSeedUp(); };
  interface.setSeedDown = [](void) -> void { sequence.setSeedDown(); };
  interface.setOctavesUp = [](void) -> void { sequence.setOctavesUp(); };
  interface.setOctavesDown = [](void) -> void { sequence.setOctavesDown(); };

  interface.stepPosition = &scheduler.currentNote;
  interface.sequenceLength = &sequence.sequenceLength;
  interface.notes = sequence.notes;
  interface.random_root = &sequence.currentRoot;
  interface.random_scale = &sequence.currentScale;
  interface.random_seed = &sequence.currentSeed;
  interface.random_octaves = &sequence.currentOctaves;
  interface.onChangeClockSource = [](int i) -> void { scheduler.setClockSource(i); };
  interface.onChangeBPM = [](void) -> void { scheduler.onBPMChange(); };
  interface.BPM = &(scheduler.BPM);
  interface.setup();
  Serial.println("Interface setup done");

  scheduler.init();
  scheduler.notes = sequence.notes;
  scheduler.sequenceLength = &sequence.sequenceLength;

  usbMIDI.setHandleClock(handleClock);
  usbMIDI.setHandleStart(handleStart);
  delay(500);
  interface.renderSplash();
  Serial.println("all setup complete");
}
void loop()
{
  // unsigned long startTime = micros();
  usbMIDI.read();
  interface.update();
  // delay(100);
  // Serial.println("loop");
  // meanLoopTime = meanLoopTime * loops / (loops + 1) + (millis() - startTime) / (loops + 1.0);
  // loops += 1;
  // if (loops % 1000 == 0)
  // {
  //   Serial.printf("mean loop time %d\n", micros() - startTime);
  // }
}