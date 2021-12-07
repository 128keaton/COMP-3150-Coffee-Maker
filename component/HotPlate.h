//
// Created by Keaton Burleson on 12/7/21.
//

#ifndef COFFEEMAKER_HOTPLATE_H
#define COFFEEMAKER_HOTPLATE_H

#include <functional>
#include <string>

#include "Carafe.h"
#include "../type/Heater.h"

using std::function, std::string;

class HotPlate : private Heater {
public:

    function<void(string)> statusCallback = nullptr;

    HotPlate() : Heater(50.0) {};

    void updateState(Carafe &carafe, const function<void(double)>& hotplateStatusCallback) {
        bool shouldBeHeating = (carafe.isAvailable() && !carafe.isEmpty() && this->temperatureValue < this->maxTemperature);

        if (shouldBeHeating && !this->isHeating) {
            this->updateState();
            this->startHeating(hotplateStatusCallback);
            this->updateState();
        } else if (!shouldBeHeating && this->isHeating) {
            this->isHeating = false;
            this->updateState();
        }
    }

    [[maybe_unused]] string getReadableHotPlateInfo() {
        return this->getReadableStatus();
    }

    double & getMaxTemperature() {
        return this->maxTemperature;
    }

private:
    void updateState() {
        if (this->statusCallback != nullptr) {
            this->statusCallback(this->getReadableStatus());
        }
    }
};

#endif //COFFEEMAKER_HOTPLATE_H
