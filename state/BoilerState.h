//
// Created by Keaton Burleson on 12/2/21.
//

#ifndef COFFEEMAKER_BOILER_STATE_H
#define COFFEEMAKER_BOILER_STATE_H
#include "HeaterState.h"

struct BoilerState: HeaterState {
    double waterLevel;
};

#endif //COFFEEMAKER_BOILER_STATE_H
