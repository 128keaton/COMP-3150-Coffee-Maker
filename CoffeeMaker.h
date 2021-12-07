//
// Created by Keaton Burleson on 12/1/21.
//

#ifndef COFFEEMAKER_COFFEEMAKER_H
#define COFFEEMAKER_COFFEEMAKER_H


#include <iostream>
#include <string>
#include <functional>
#include "sleep.h"
#include "component/Boiler.h"
#include "component/Carafe.h"
#include "component/HotPlate.h"
#include "state/CoffeeMakerState.h"
#include "helpers/Indicators.hpp"

using std::cout, std::endl, std::string, std::function;

class CoffeeMaker {
public:
    CoffeeMaker();

    CoffeeMakerState * getState();
    void updateState();

    void fillBoiler(const function<void(string)> &capacityInfoCallback = nullptr);
    void startBoiling(const function<void(string)> &heaterInfoCallback = nullptr);

    void emptyCarafe();
    void removeCarafe();
    void replaceCarafe();
    void pourCup();

    void brew();

    string getHotPlateInfo();
    double availableCoffee();

    function<void(string)> hotplateStateUpdated = nullptr;
private:
    CoffeeMakerState currentState;

    Boiler boiler = Boiler();
    Carafe carafe = Carafe();
    HotPlate hotPlate = HotPlate();

    void updateHotplateState();
    static void resetConsole();
};


#endif //COFFEEMAKER_COFFEEMAKER_H
