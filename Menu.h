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
    int currentString = 0;

    Graphics *disp;
    std::vector<Screen *> nextScreens;
    std::vector<std::function<void(std::vector<int> *stack)> > callbacks;
    std::string title;
    Screen(const char *t)
    {
        title = std::string(t);
    }

    void onClick(std::vector<int> *stack)
    {
        if (callbacks[currentString] != nullptr)
        {
            callbacks[currentString](stack);
        }
    }

    void onIncrement()
    {
        currentString = (currentString + 1) % strings.size();
        renderScreen();
    }

    Screen *nextScreen()
    {
        return nextScreens[currentString];
    }

    void onDecrement()
    {
        currentString = (currentString - 1 + strings.size()) % strings.size();
        renderScreen();
    }

    void renderScreen()
    {
        disp->buildTwoStringScreen(title.c_str(), strings[currentString].c_str());
    }
};

class Menu
{
public:
    Screen *currentScreen;
    Screen *previousScreen;
    Graphics *disp;
    std::vector<int> paramterStack = {};
    std::vector<Screen> screens = {};
    Menu()
    {
        Screen loadSaveScreen("BANK");
        loadSaveScreen.strings.push_back("0");
        loadSaveScreen.strings.push_back("1");
        loadSaveScreen.strings.push_back("2");
        loadSaveScreen.strings.push_back("3");

        Screen loadSaveOperationScreen("ACTION");
        loadSaveOperationScreen.strings.push_back("LOAD");
        loadSaveOperationScreen.strings.push_back("SAVE");
        loadSaveOperationScreen.strings.push_back("BACK");

        screens.push_back(loadSaveScreen);
        screens.push_back(loadSaveOperationScreen);
        currentScreen = &(screens[0]);
        previousScreen = &(screens[0]);
        screens[0].nextScreens = {&(screens[1]), &(screens[1]), &(screens[1]), &(screens[1])};
        screens[1].nextScreens = {nullptr, nullptr, nullptr};
    }
    void init()
    {
        screens[0].callbacks = {nullptr, nullptr, nullptr, nullptr};
        screens[1].callbacks = {onLoad, onSave, nullptr};
    }
    void setGraphicsPointer(Graphics *_d)
    {
        currentScreen->disp = _d;
    }

    void onClick()
    {
        int m = currentScreen->currentString;
        currentScreen->onClick(&paramterStack);
        Screen *nextScreen;
        if (currentScreen->nextScreen() == nullptr)
        {
            paramterStack.empty();
            while (!paramterStack.empty())
                paramterStack.pop_back();
            nextScreen = previousScreen;
        }
        else
        {
            nextScreen = currentScreen->nextScreen();
            previousScreen = currentScreen;
            paramterStack.push_back(m);
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
};

#endif
