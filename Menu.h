#include <stdlib.h>
#include <functional>
#include <vector>
#include "Arduino.h"

#ifndef _MENU_H_
#define _MENU_H_

class Screen
{
public:
    std::vector<std::string> strings;
    unsigned int currentString = 0;

    Graphics *disp;
    std::vector<Screen *> nextScreens;
    std::vector<std::function<void(std::vector<int> *stack)> > callbacks;
    std::string title;
    float *controledParam = nullptr;
    Screen(const char *t, float *parameter)
    {
        title = std::string(t);
        controledParam = parameter;
    }
    Screen(const char *t)
    {
        title = std::string(t);
    }

    void onClick(std::vector<int> *stack)
    {
        if (currentString < callbacks.size() && callbacks[currentString] != nullptr)
        {
            callbacks[currentString](stack);
        }
    }

    void onIncrement()
    {
        if (controledParam != nullptr)
        {
            (*controledParam) += 1;
        }
        else
        {
            currentString = (currentString + 1) % strings.size();
        }
        renderScreen();
    }

    Screen *nextScreen()
    {
        return nextScreens[currentString];
    }

    void onDecrement()
    {
        if (controledParam != nullptr)
        {
            (*controledParam) -= 1;
        }
        else
        {
            currentString = (currentString - 1 + strings.size()) % strings.size();
        }
        renderScreen();
    }

    void renderScreen()
    {
        if (disp == nullptr)
        {
            Serial.println("DISP POINTER IS NULL ON SCREEN");
        }
        if (controledParam != nullptr)
        {
            disp->buildTwoStringScreen(title.c_str(), String(*controledParam).c_str());
        }
        else
        {
            disp->buildTwoStringScreen(title.c_str(), strings[currentString].c_str());
        }
    }
};

class Menu
{
public:
    Screen *currentScreen;
    Screen *rootScreen;
    Graphics *disp;
    std::vector<int> paramterStack = {};
    std::vector<Screen> screens = {};
    float *clockBPM;
    Menu()
    {
        Screen mainMenuScreen("MENU");
        mainMenuScreen.strings.push_back("BANKS");
        mainMenuScreen.strings.push_back("CLOCK");

        Screen loadSaveScreen("BANK");
        loadSaveScreen.strings.push_back("1");
        loadSaveScreen.strings.push_back("2");
        loadSaveScreen.strings.push_back("3");
        loadSaveScreen.strings.push_back("4");
        loadSaveScreen.strings.push_back("5");
        loadSaveScreen.strings.push_back("6");
        loadSaveScreen.strings.push_back("7");
        loadSaveScreen.strings.push_back("8");

        Screen loadSaveOperationScreen("ACTION");
        loadSaveOperationScreen.strings.push_back("LOAD");
        loadSaveOperationScreen.strings.push_back("SAVE");
        loadSaveOperationScreen.strings.push_back("MASK");
        loadSaveOperationScreen.strings.push_back("BACK");

        Screen clockScreen("CLOCK");
        clockScreen.strings.push_back("SOURCE");
        clockScreen.strings.push_back("BPM");

        Screen clockSourceScreen("CLK SOURCE");
        clockSourceScreen.strings.push_back("EXTERNAL");
        clockSourceScreen.strings.push_back("INTERNAL");

        Screen clockBPMScreen("CLK BPM", clockBPM);

        screens.push_back(mainMenuScreen);          // 0
        screens.push_back(loadSaveScreen);          // 1
        screens.push_back(loadSaveOperationScreen); // 2
        screens.push_back(clockScreen);             // 3
        screens.push_back(clockSourceScreen);       // 4
        screens.push_back(clockBPMScreen);          // 5

        currentScreen = &(screens[0]);
        rootScreen = &(screens[0]);
        screens[0].nextScreens = {&(screens[1]), &(screens[3])};
        screens[1].nextScreens = {&(screens[2]), &(screens[2]), &(screens[2]), &(screens[2]), &(screens[2]), &(screens[2]), &(screens[2]), &(screens[2])};
        screens[2].nextScreens = {nullptr, nullptr, nullptr, nullptr};
        screens[3].nextScreens = {&(screens[4]), &(screens[5])};
        screens[4].nextScreens = {nullptr, nullptr};
        screens[5].nextScreens = {nullptr, nullptr};
    }
    void init()
    {
        screens[0].callbacks = {nullptr, nullptr};
        screens[1].callbacks = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        screens[2].callbacks = {onLoad, onSave, onMask, nullptr};
        screens[3].callbacks = {nullptr, nullptr};
        screens[4].callbacks = {onChangeClockSource, onChangeClockSource};
        screens[5].callbacks = {onChangeBPM};
    }
    void setGraphicsPointer(Graphics *_d)
    {
        for (unsigned int i = 0; i < screens.size(); i++)
        {
            screens[i].disp = _d;
        }
    }

    void printStack()
    {
        for (unsigned char i = 0; i < paramterStack.size(); i++)
        {
            Serial.printf("%d ", paramterStack[i]);
        }
        Serial.print("\n");
    }

    void setBPMPointer(float *b)
    {
        screens[5].controledParam = b;
    }

    void onClick()
    {
        int m = currentScreen->currentString;
        paramterStack.push_back(m);
        // printStack();
        currentScreen->onClick(&paramterStack);
        Screen *nextScreen;
        if (currentScreen->nextScreen() == nullptr)
        {
            Serial.println("next screen is empty");
            paramterStack.empty();
            while (!paramterStack.empty())
                paramterStack.pop_back();
            nextScreen = rootScreen;
        }
        else
        {
            nextScreen = currentScreen->nextScreen();
        }
        currentScreen = nextScreen;
        currentScreen->renderScreen();
    }
    void onIncrement()
    {
        currentScreen->onIncrement();
    }
    void onDecrement()
    {
        currentScreen->onDecrement();
    }
    std::function<void(std::vector<int> *)> onLoad;
    std::function<void(std::vector<int> *)> onSave;
    std::function<void(std::vector<int> *)> onMask;
    std::function<void(std::vector<int> *)> onChangeClockSource;
    std::function<void(std::vector<int> *)> onChangeBPM;
};

#endif
