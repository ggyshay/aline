
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
    unsigned char currentPage = 0;
    bool pressedAuxButtons[8];
    unsigned char selectionStart = 0;
    unsigned char selectionEnd = 0;
    unsigned long lastSelectionChange = 0;

    unsigned char visualisationMode = LED_STEP_MODE;
    unsigned char displayMode = 1;
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
    void writeLedModes(unsigned char i);

public:
    int *stepPosition;
    unsigned char *sequenceLength;
    char *pagesLength;
    Note *notes;
    unsigned char *random_root;
    unsigned char *random_scale;
    char *random_seed;
    unsigned char *random_octaves;
    float *BPM;
    std::function<void(void)> onLengthUp = nullptr;
    std::function<void(void)> onLengthDown = nullptr;
    std::function<void(void)> onDurationUp = nullptr;
    std::function<void(void)> onDurationDown = nullptr;
    std::function<void(void)> onVelocityUp = nullptr;
    std::function<void(void)> onVelocityDown = nullptr;
    std::function<void(void)> onPitchUp = nullptr;
    std::function<void(void)> onPitchDown = nullptr;
    std::function<void(void)> onOctUp = nullptr;
    std::function<void(void)> onOctDown = nullptr;
    std::function<void(unsigned char, unsigned char)> onSelectionChange = nullptr;
    std::function<void(void)> onCopy = nullptr;
    std::function<void(void)> onPaste = nullptr;
    std::function<void(void)> onErase = nullptr;
    std::function<void(bool *, unsigned char)> onGateChange = nullptr;
    std::function<void(void)> onEase = nullptr;
    std::function<void(int)> onLoad = nullptr;
    std::function<void(int)> onSave = nullptr;
    std::function<void(int)> onMask = nullptr;
    std::function<void(void)> setSeedUp = nullptr;
    std::function<void(void)> setSeedDown = nullptr;
    std::function<void(void)> setScaleUp = nullptr;
    std::function<void(void)> setScaleDown = nullptr;
    std::function<void(void)> setRootUp = nullptr;
    std::function<void(void)> setRootDown = nullptr;
    std::function<void(void)> setOctavesUp = nullptr;
    std::function<void(void)> setOctavesDown = nullptr;
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

void getMinMaxNotes(Note *notes, std::function<unsigned char(Note n)> extractor, Note *minNote, Note *maxNote, unsigned char start, unsigned char end);