#include <iostream>
#include <string>
#include <functional>
#include "CoffeeMaker.h"

using std::cout, std::endl, std::cin, std::string, std::to_string;

string getReadableState(CoffeeMakerState state);

int runCoffeeMaker(CoffeeMaker *coffeeMaker);

bool promptUser(const string &message);

void printPrefix(const string &message, const string &prefix, bool newline = true, const string &component = "");

void performCorrectiveAction(CoffeeMakerState &state, CoffeeMaker *coffeeMaker);

int main() {
    CoffeeMaker coffeeMaker = CoffeeMaker();

    return runCoffeeMaker(&coffeeMaker);
}

int runCoffeeMaker(CoffeeMaker *coffeeMaker) {
    CoffeeMakerState *currentState = coffeeMaker->getState();

    coffeeMaker->hotplateStateUpdated = [](const string &hotplateState) {
        printPrefix(hotplateState, "INFO", true, "Hot Plate");
    };

    while (*currentState != READY) {
        performCorrectiveAction(*currentState, coffeeMaker);
    }

    printPrefix(getReadableState(*currentState), "INFO");

    coffeeMaker->brew();

    while (true) {
        if (!promptUser("Would you like a cup?") || (coffeeMaker->availableCoffee() <= 0)) {
            break;
        }

        printPrefix("Pouring a cup of coffee for you...", "ACTION");
        coffeeMaker->pourCup();
        printPrefix(to_string(coffeeMaker->availableCoffee()) + " coffee remaining", "INFO");
    }


    if (promptUser("Would you like to brew another pot?")) {
        return runCoffeeMaker(coffeeMaker);
    }

    return 0;
}

void performCorrectiveAction(CoffeeMakerState &state, CoffeeMaker *coffeeMaker) {
    const auto printIssue = [](const string &message) {
        printPrefix(message, "ISSUE");
    };

    const auto printInfo = [](const string &message, const string &component = "") {
        printPrefix(message, "INFO", true, component);
    };

    const auto printAction = [](const string &message) {
        printPrefix(message, "ACTION");
    };

    switch (state) {
        case WAITING:
        case READY:
            printInfo(getReadableState(state));
            printInfo(coffeeMaker->getHotPlateInfo(), "Hot Plate");
            break;
        case BOILING:
            printInfo(getReadableState(state));
            break;
        case BOILER_LOW:
            printIssue(getReadableState(state));
            printAction("Filling boiler...");
            coffeeMaker->fillBoiler([&printInfo](const string &boilerCapacityInfo) {
                printInfo(boilerCapacityInfo, "Boiler");
            });
            break;
        case CARAFE_FULL:
            printIssue(getReadableState(state));
            printAction("Emptying carafe...");
            coffeeMaker->emptyCarafe();
            printInfo(coffeeMaker->getHotPlateInfo(), "Hot Plate");
            break;
        case CARAFE_MISSING:
            printIssue(getReadableState(state));
            printAction("Replacing carafe...");
            coffeeMaker->replaceCarafe();
            printInfo(coffeeMaker->getHotPlateInfo(), "Hot Plate");
            break;
        case BOILER_COOL:
            printIssue(getReadableState(state));
            printAction("Boiling water..");
            coffeeMaker->startBoiling([&printInfo](const string &boilerHeaterInfo) {
                printInfo(boilerHeaterInfo, "Boiler");
            });
            break;
    }
}


string getReadableState(CoffeeMakerState state) {
    switch (state) {
        case WAITING:
        default:
            return "Waiting for other components";
        case READY:
            return "Ready to brew!";
        case BOILER_LOW:
            return "Boiler needs to be filled";
        case BOILING:
            return "The boiler is currently boiling the water";
        case CARAFE_FULL:
            return "The carafe is already full of coffee!";
        case CARAFE_MISSING:
            return "The carafe is not in the machine";
        case BOILER_COOL:
            return "The boiler's contents are not warm enough";
    }
}


bool promptUser(const string &message) {
    char result;
    bool inLoop = true;

    while (inLoop) {
        printPrefix(message + " (Y/N) ", "PROMPT", false);
        cin >> result;

        if (result == 'N' || result == 'n' || result == 'Y' || result == 'y') {
            inLoop = false;
        }
    }

    if (result == 'N' || result == 'n') {
        return false;
    }

    return true;
}

void printPrefix(const string &message, const string &prefix, bool newline, const string &component) {
    cout << "[" << prefix;

    if (!component.empty()) {
        cout << "][" << component;
    }

    cout << "]: " << message;

    if (newline) {
        cout << endl;
    }
}