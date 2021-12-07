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

class Boiler : public Fillable, public Heatable {
public:

    function<void(BoilerState)> boilerStateFunction = nullptr;

    using StateChanges<HeaterState>::stateFunction;

    explicit Boiler(double maxCapacity = 300) :
            Heatable(),
            Fillable(maxCapacity) {

        this->stateFunction = [this](HeaterState heaterState) {
            this->set(this->boilerState.waterLevel);
        };

        this->updated = [this](double waterLevel) {
            HeaterState state = this->getHeaterState();

            this->stateUpdated(
                    {
                            state.heating,
                            (waterLevel == 0.0 ? 0.0 : state.currentTemperature),
                    }
            );

            this->boilerStateUpdated(
                    {
                            state.heating,
                            state.currentTemperature,
                            waterLevel
                    }
            );
        };
    }

    bool contentsHeated() {
        return this->getHeaterState().currentTemperature == HEATER_MAX_TEMP;
    }

    void fill(const function<void(double)> &statusCallback, int fillSpeed) override {
        Fillable::fill(statusCallback, fillSpeed);
        this->heaterSensor.set(0.0);
    }

    BoilerState &getBoilerState() {
        return this->boilerState;
    }

private:
    BoilerState boilerState = {};

    void boilerStateUpdated(BoilerState state) {
        this->boilerState = state;
        if (this->boilerStateFunction != nullptr) {
            this->boilerStateFunction(state);
        }
    }
};


#endif //COFFEEMAKER_BOILER_H
