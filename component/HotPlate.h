//
// Created by Keaton Burleson on 12/7/21.
//

#ifndef COFFEEMAKER_HOTPLATE_H
#define COFFEEMAKER_HOTPLATE_H

#include <functional>
#include <chrono>
#include <future>
#include <string>

#include "Carafe.h"
#include "../type/Heater.h"

using std::async, std::this_thread::sleep_for, std::function, std::string;

class HotPlate : private Heater {
public:

    function<void(string)> statusCallback = nullptr;

    HotPlate() : Heater(50.0) {};

    void updateState(Carafe &carafe) {
        bool shouldBeHeating = (carafe.isAvailable() && !carafe.isEmpty() && this->temperatureValue < this->maxTemperature);

        if (shouldBeHeating && !this->isHeating) {
            using namespace std::chrono_literals;

            auto heaterFunction = async(std::launch::async, [this] {
                this->startHeating(nullptr, 25);
                this->updateState();
                return false;
            });

            this->isHeating = heaterFunction.get();
        } else if (!shouldBeHeating && this->isHeating) {
            this->isHeating = false;
            this->updateState();
        }
    }

    [[maybe_unused]] string getReadableHotPlateInfo() {
        return this->getReadableStatus();
    }

private:
    void updateState() {
        if (this->statusCallback != nullptr) {
            this->statusCallback(this->getReadableStatus());
        }
    }
};

#endif //COFFEEMAKER_HOTPLATE_H
