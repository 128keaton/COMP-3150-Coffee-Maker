//
// Created by Keaton Burleson on 12/2/21.
//

#ifndef COFFEEMAKER_CARAFE_H
#define COFFEEMAKER_CARAFE_H

#include "../type/Fillable.h"
#include "../type/Readable.h"
#include "../state/CarafeState.h"
#include "../type/StateChanges.h"

class Carafe : public Fillable, public StateChanges<CarafeState> {
public:
    explicit Carafe(const function<void(CarafeState)> &stateChanged) : Fillable(100, [this](double coffeeLevel) {
        const bool carafeAvailable = this->carafeSensor.get();

        this->stateUpdated(
                {
                        carafeAvailable,
                        coffeeLevel,
                }
        );
    }), StateChanges<CarafeState>(stateChanged) {}

    double & currentCapacity() {
        return this->get();
    }

    void replace() {
        this->carafeSensor.set(true);
    }

    void remove() {
        this->carafeSensor.set(false);
    }

    Readable<bool> carafeSensor = Readable<bool>(false, [this](bool isAvailable) {
        this->stateUpdated(
                {
                        isAvailable,
                        this->get(),
                }
        );
    });
};

#endif //COFFEEMAKER_CARAFE_H
