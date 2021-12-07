//
// Created by Keaton Burleson on 12/1/21.
//

#include "CoffeeMaker.h"

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
    this->updateHotplateState();

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
    using namespace indicators;

    auto createPostfixText = [this]() -> string {
        return "Filled " + this->boiler.getReadableBoilerCapacityInfo();
    };

    BlockProgressBar bar{
            option::BarWidth{100},
            option::Start{"["},
            option::End{" ]"},
            option::PostfixText{createPostfixText()},
            option::ForegroundColor{Color::cyan},
            option::ShowPercentage(false),
            option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
    };

    this->boiler.fill([&bar, this, &createPostfixText](double val) {
        bar.set_progress((float) ((val / this->boiler.getMaxCapacity()) * 100));
        bar.set_option(option::PostfixText{createPostfixText()});
    }, 20);

    CoffeeMaker::resetConsole();

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
    using namespace indicators;

    auto createPostfixText = [this]() -> string {
        return "Boiler Temperature " + this->boiler.getReadableBoilerHeaterInfo();
    };

    BlockProgressBar bar{
            option::BarWidth{this->boiler.getMaxTemperature()},
            option::Start{"["},
            option::End{" ]"},
            option::PostfixText{createPostfixText()},
            option::ForegroundColor{Color::red},
            option::ShowPercentage(false),
            option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
    };

    this->boiler.startBoiler([&bar, &createPostfixText](double val) {
        bar.set_progress((float) val);
        bar.set_option(option::PostfixText{createPostfixText()});
    });

    CoffeeMaker::resetConsole();

    if (heaterInfoCallback != nullptr) {
        heaterInfoCallback(this->boiler.getReadableBoilerHeaterInfo());
    }

    this->updateState();
}

void CoffeeMaker::brew() {
    const double maxCapacity = this->carafe.getMaxCapacity();
    double fillAmount = 10;

    using namespace indicators;

    BlockProgressBar bar{
            option::BarWidth{maxCapacity},
            option::Start{"["},
            option::End{" ]"},
            option::PostfixText{"Brewing..."},
            option::ForegroundColor{Color::magenta},
            option::ShowPercentage(true),
            option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
    };

    while (this->carafe.getCurrentCapacity() < this->carafe.getMaxCapacity()) {
        bar.set_progress((float) (this->carafe.getCurrentCapacity() + fillAmount));
        this->carafe.brewFrom((Tank &) (boiler), fillAmount);
        millisleep(150);
    }

    CoffeeMaker::resetConsole();
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

void CoffeeMaker::resetConsole() {
    cout << termcolor::reset << endl;
}

void CoffeeMaker::updateHotplateState() {
    using namespace indicators;

    auto createPostfixText = [this]() -> string {
        return "Hot Plate " + this->hotPlate.getReadableHotPlateInfo();
    };

    BlockProgressBar bar{
            option::BarWidth{this->hotPlate.getMaxTemperature()},
            option::Start{"["},
            option::End{" ]"},
            option::PostfixText{createPostfixText()},
            option::ForegroundColor{Color::red},
            option::ShowPercentage(false),
            option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
    };

    this->hotPlate.updateState(this->carafe, [&bar, &createPostfixText](double val) {
        bar.set_progress((float) val);
        bar.set_option(option::PostfixText{createPostfixText()});
    });

    CoffeeMaker::resetConsole();
}
