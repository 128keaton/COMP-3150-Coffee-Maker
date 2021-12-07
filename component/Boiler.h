//
// Created by Keaton Burleson on 12/1/21.
//

#ifndef COFFEEMAKER_BOILER_H
#define COFFEEMAKER_BOILER_H

#include "../type/Tank.h"
#include "../type/Heater.h"


class Boiler : private Tank, private Heater {
public:

    explicit Boiler(double maxCapacity = 300, double maxTemperature = 100.0) : Tank(maxCapacity),  Heater(maxTemperature) {}

    double & getWaterLevel() {
        return this->currentCapacity;
    }

    double & getMaxCapacity() {
        return this->maxCapacity;
    }

    bool contentsHeated() {
        return this->temperatureValue == this->maxTemperature;
    }

    bool & heaterOn() {
        return this->isHeating;
    }

    void fill(const function<void(double)> &statusCallback, int fillSpeed) override {
        Tank::fill(statusCallback, fillSpeed);
        Heater::reset();
    }

    void startBoiler(const function<void(double)> &statusCallback) {
        Heater::startHeating(statusCallback);
    }

    string getReadableBoilerCapacityInfo() {
        return Tank::getReadableStatus();
    }

    string getReadableBoilerHeaterInfo() {
        return Heater::getReadableStatus();
    }
};


#endif //COFFEEMAKER_BOILER_H
