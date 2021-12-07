//
// Created by Keaton Burleson on 12/6/21.
//

#ifndef COFFEEMAKER_STATE_H
#define COFFEEMAKER_STATE_H
enum CoffeeMakerState {
    READY,
    WAITING,
    BOILER_LOW,
    BOILER_COOL,
    BOILING,
    CARAFE_FULL,
    CARAFE_MISSING
};
#endif //COFFEEMAKER_STATE_H
