//
// Created by Keaton Burleson on 12/1/21.
//

#include "CoffeeMaker.h"
#include "component/Boiler.h"
#include "helpers/ProgressBar.h"


CoffeeMaker::CoffeeMaker() {
    this->currentState = WAITING;

    this->hotPlate.statusCallback = [this](const string &hotplateState) {
        if (this->hotplateStateUpdated != nullptr) {
            this->hotplateStateUpdated(hotplateState);
        }
    };

    this->emptyCarafe();
}

void CoffeeMaker::updateState() {
    this->hotPlate.updateState(this->carafe);

    auto determineState = [this]() -> CoffeeMakerState {
        if (!this->carafe.isAvailable()) {
            return CARAFE_MISSING;
        } else if (this->carafe.getCurrentCapacity() == this->carafe.getMaxCapacity()) {
            return CARAFE_FULL;
        } else if (this->boiler.getWaterLevel() == 0) {
            return BOILER_LOW;
        } else if (this->boiler.heaterOn()) {
            return BOILING;
        } else if (!this->boiler.contentsHeated()) {
            return BOILER_COOL;
        }

        return READY;
    };

    this->currentState = determineState();
}

CoffeeMakerState *CoffeeMaker::getState() {
    return &this->currentState;
}

void CoffeeMaker::fillBoiler(const function<void(string)> &capacityInfoCallback) {
    this->boiler.fill([this](double val) {
        bool isLast = val == this->boiler.getMaxCapacity();
        ProgressBar::percentage(val, this->boiler.getMaxCapacity(), "Filled", isLast);
    }, 20);

    if (capacityInfoCallback != nullptr) {
        capacityInfoCallback(this->boiler.getReadableBoilerCapacityInfo());
    }

    this->updateState();
}

void CoffeeMaker::emptyCarafe() {
    this->removeCarafe();
    this->carafe.empty();
    this->replaceCarafe();
}

void CoffeeMaker::removeCarafe() {
    this->carafe.remove();
    this->updateState();
}

void CoffeeMaker::replaceCarafe() {
    this->carafe.replace();
    this->updateState();
}

void CoffeeMaker::startBoiling(const function<void(string)> &heaterInfoCallback) {
    this->boiler.startBoiler([](double val) {
        ProgressBar::temperature(val, 100.0, "Boiler Temperature", (val == 100.0));
    });

    if (heaterInfoCallback != nullptr) {
        heaterInfoCallback(this->boiler.getReadableBoilerHeaterInfo());
    }

    this->updateState();
}

void CoffeeMaker::brew() {
    const double maxCapacity = this->carafe.getMaxCapacity();
    double fillAmount = 10;

    while (this->carafe.getCurrentCapacity() < this->carafe.getMaxCapacity()) {
        ProgressBar::percentage((int) (this->carafe.getCurrentCapacity() + fillAmount), (int) maxCapacity, "Brewing...",
                                (this->carafe.getCurrentCapacity() + fillAmount) >= 100);
        this->carafe.brewFrom((Tank &) (boiler), fillAmount);
        usleep(15 * 5000 * 10);
    }

    this->updateState();
}

void CoffeeMaker::pourCup() {
    this->removeCarafe();
    this->carafe.pour(10.0);
    this->replaceCarafe();
}

double CoffeeMaker::availableCoffee() {
    return this->carafe.getCurrentCapacity();
}

string CoffeeMaker::getHotPlateInfo() {
    return this->hotPlate.getReadableHotPlateInfo();
}
