//
// Created by Keaton Burleson on 12/1/21.
//

#ifndef COFFEEMAKER_COFFEEMAKER_H
#define COFFEEMAKER_COFFEEMAKER_H

#include <iostream>
#include <string>
#include <functional>
#include "type/Readable.h"
#include "component/Boiler.h"
#include "component/Carafe.h"
#include "state/CoffeeMakerState.h"

using std::cout, std::endl, std::string, std::function;

class CoffeeMaker {
public:
    CoffeeMaker();

    CoffeeMakerState * getState();

    void fillBoiler(const function<void(string)> &capacityInfoCallback = nullptr);
    void startBoiling(const function<void(string)> &heaterInfoCallback = nullptr);

    void emptyCarafe();
    void removeCarafe();
    void replaceCarafe();
    void pourCup();

    void brew();

    double availableCoffee();
private:

    CoffeeMakerState currentState;

    CoffeeMakerState updateState(CarafeState carafeState, BoilerState boilerState);

    Boiler boiler = Boiler([](BoilerState state) {
     //   this->currentState = this->updateState(this->carafe.getState(), state);
    });

    Carafe carafe = Carafe([](CarafeState state) {
     //   this->currentState = this->updateState(state, this->boiler.getState());
    });
};


#endif //COFFEEMAKER_COFFEEMAKER_H
