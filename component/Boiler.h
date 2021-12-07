//
// Created by Keaton Burleson on 12/1/21.
//

#ifndef COFFEEMAKER_BOILER_H
#define COFFEEMAKER_BOILER_H

#include "../type/Fillable.h"
#include "../type/Readable.h"
#include "../type/Heatable.h"
#include "../state/BoilerState.h"
#include "../type/StateChanges.h"

const double HEATER_MAX_TEMP = 100.0;

class Boiler : public Fillable, public Heatable, public StateChanges<BoilerState> {
public:
    using StateChanges<BoilerState>::stateUpdated;
    using StateChanges<BoilerState>::getState;

    explicit Boiler(const function<void(BoilerState)> &stateChanged, double maxCapacity = 300) :
            Fillable(maxCapacity, [this](double waterLevel) {
                HeaterState state = this->getHeaterState();

                this->stateUpdated(
                        {
                                state.heating,
                                (waterLevel == 0.0 ? 0.0 : state.currentTemperature),
                                waterLevel
                        }
                );
            }),
            Heatable([this](HeaterState heaterState) {
                const BoilerState currentState = this->getState();

                this->stateUpdated(
                        {
                                heaterState.heating,
                                heaterState.currentTemperature,
                                currentState.waterLevel
                        }
                );

                this->set(currentState.waterLevel);
            }), StateChanges<BoilerState>(stateChanged) {

        this->stateUpdated(
                {
                        false,
                        0.0,
                        0.0
                }
        );
    }

    bool contentsHeated() {
        return this->getHeaterState().currentTemperature == HEATER_MAX_TEMP;
    }

    void fill(const function<void(double)> &statusCallback, int fillSpeed) override {
        Fillable::fill(statusCallback, fillSpeed);
        this->heaterSensor.set(0.0);
    }
};


#endif //COFFEEMAKER_BOILER_H
