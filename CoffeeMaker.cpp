//
// Created by Keaton Burleson on 12/1/21.
//

#include "CoffeeMaker.h"
#include "component/Boiler.h"
#include "helpers/ProgressBar.h"


CoffeeMaker::CoffeeMaker() {
    this->currentState = WAITING;
    this->carafe.empty();
}

CoffeeMakerState CoffeeMaker::updateState(CarafeState carafeState, BoilerState boilerState) {
    if (!carafeState.available) {
        return CARAFE_MISSING;
    } else if (carafeState.coffeeLevel == this->carafe.getMaxCapacity()) {
        return CARAFE_FULL;
    } else if (boilerState.waterLevel == 0) {
        return BOILER_LOW;
    } else if (boilerState.heating) {
        return BOILING;
    } else if (!this->boiler.contentsHeated()) {
        return BOILER_COOL;
    }

    return READY;
}

CoffeeMakerState * CoffeeMaker::getState() {
    return &this->currentState;
}

void CoffeeMaker::fillBoiler(const function<void(string)> &capacityInfoCallback) {
    this->boiler.fill([this](double val) {
        bool isLast = val == this->boiler.getMaxCapacity();
        ProgressBar::percentage(val, this->boiler.getMaxCapacity(), "Filled", isLast);
    }, 20);

    if (capacityInfoCallback != nullptr) {
        capacityInfoCallback(this->boiler.readableFillStatus());
    }
}

void CoffeeMaker::emptyCarafe() {
    this->carafe.empty();
}

void CoffeeMaker::removeCarafe() {
    this->carafe.remove();
}

void CoffeeMaker::replaceCarafe() {
    this->carafe.replace();
}

void CoffeeMaker::startBoiling(const function<void(string)> &heaterInfoCallback) {
    this->boiler.startHeating([](double val) {
        ProgressBar::temperature(val, 100.0, "Boiler Temperature", (val == 100.0));
    });

    if (heaterInfoCallback != nullptr) {
        heaterInfoCallback(this->boiler.readableHeaterStatus());
    }
}

void CoffeeMaker::brew() {
    const double maxCapacity = this->carafe.getMaxCapacity();
    double fillAmount = 10;

    while(this->carafe.currentCapacity() < this->carafe.getMaxCapacity()) {
        ProgressBar::percentage((int) (this->carafe.currentCapacity() + fillAmount), (int) maxCapacity, "Brewing...", (this->carafe.get() + fillAmount) >= 100);
        this->carafe.addFrom(boiler, fillAmount);
        usleep(15 * 5000 * 10);
    }
}

void CoffeeMaker::pourCup() {
    this->removeCarafe();
    this->carafe.take(10.0);
    this->replaceCarafe();
}

double CoffeeMaker::availableCoffee() {
    return this->carafe.currentCapacity();
}
