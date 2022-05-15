
#include "Arduino.h"
#include "Encoder.h"
#include "Button.h"

#include "Graphics.h"
#include "Menu.h"
#include <stdlib.h>
#include <functional>
#include "Note.h"
#include "HAL.h"
#define SELECTION_CHANGE_DELAY 700
#define LED_STEP_MODE 0
#define LED_SELECTION_MODE 1
#define LED_LENGTH_MODE 2
#define DISPLAY_SELECTION_MODE 0
#define DISPLAY_VELOCITY_MODE 1
#define DISPLAY_DURATION_MODE 2
#define DISPLAY_LENGTH_MODE 3
#define DISPLAY_GATE_MODE 4
#define DISPLAY_RANDOM_ROOT_MODE 5
#define DISPLAY_RANDOM_SCALE_MODE 6
#define DISPLAY_RANDOM_SEED_MODE 7
#define DISPLAY_RANDOM_OCTAVES_MODE 8
#define VISUALISATION_CHANGE_DEBOUNCE 2000

class Interface
{
private:
    Encoder encoders[5];
    Button *buttons[16];
    Button *auxButtons[8];
    Graphics disp;
    Menu menu;

    bool pressedButtons[16];
    bool gateMode = false;
    bool randomMode = false;
    bool multiMode = false;
    byte currentPage = 0;
    bool pressedAuxButtons[8];
    byte selectionStart = 0;
    byte selectionEnd = 0;
    unsigned long lastSelectionChange = 0;

    byte visualisationMode = LED_STEP_MODE;
    byte displayMode = 1;
    unsigned long lastVisualisationChange = 0;

    void detectSelecion();
    void updatePageContext();
    void detectCommands();
    void setupEncodersCallbacks();
    void drawSequenceGraph();
    void printSequenceMode();
    void printVelocityMode();
    void printDurationMode();
    void printLengthMode();
    void writeToDisplay();
    void changeWriteMode(int newMode, int newDisplayMode);
    void writeLedModes(byte i);

    // void onLengthUp();
    // void onLengthDown();
    // void onDurationUp();
    // void onDurationDown();
    // void onVelocityUp();
    // void onVelocityDown();
    // void onPitchUp();
    // void onPitchDown();
    // void onOctUp();
    // void onOctDown();
    // void setSeedUp();
    // void setSeedDown();
    // void setScaleUp();
    // void setScaleDown();
    // void setRootUp();
    // void setRootDown();
    // void setOctavesUp();
    // void setOctavesDown();

public:
    byte *stepPosition;
    byte *sequenceLength;
    char *pagesLength;
    Note *notes;
    byte *random_root;
    byte *random_scale;
    char *random_seed;
    byte *random_octaves;
    float *BPM;

    std::function<void(byte t)> onMessage = nullptr;
    std::function<void(byte, byte)> onSelectionChange = nullptr;
    std::function<void(void)> onCopy = nullptr;
    std::function<void(void)> onPaste = nullptr;
    // std::function<void(void)> onErase = nullptr;
    std::function<void(bool *, byte)> onGateChange = nullptr;
    // std::function<void(void)> onEase = nullptr;
    std::function<void(int)> onLoad = nullptr;
    std::function<void(int)> onSave = nullptr;
    std::function<void(int)> onMask = nullptr;
    std::function<void(int)> onChangeClockSource;
    std::function<void(void)> onChangeBPM;
    std::function<void(int)> onActivateScaleLock;
    std::function<void(int)> onChangeScale;
    std::function<void(int)> onChangeRoot;
    std::function<void(int)> onChangeSequenceMode;
    void setup();
    void update();
    void renderSplash();
};

void getMinMaxNotes(Note *notes, std::function<byte(Note n)> extractor, Note *minNote, Note *maxNote, byte start, byte end);