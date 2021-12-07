#include <iostream>
#include <functional>

#include "CoffeeMaker.h"

using std::cout, std::endl, std::cin;

string getReadableState(CoffeeMakerState state);

int runCoffeeMaker(CoffeeMaker *coffeeMaker);

bool promptUser(const string &message);

void printPrefix(const string &message, const string &prefix, bool newline = true);

void performCorrectiveAction(CoffeeMakerState &state, CoffeeMaker *coffeeMaker);

int main() {
    CoffeeMaker coffeeMaker = CoffeeMaker();

    return runCoffeeMaker(&coffeeMaker);
}

int runCoffeeMaker(CoffeeMaker *coffeeMaker) {
    CoffeeMakerState *currentState = coffeeMaker->getState();

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

    const auto printInfo = [](const string &message) {
        printPrefix(message, "INFO");
    };

    const auto printAction = [](const string &message) {
        printPrefix(message, "ACTION");
    };

    switch (state) {
        case WAITING:
        case READY:
        case BOILING:
            cout << getReadableState(state) << endl;
            break;
        case BOILER_LOW:
            printIssue(getReadableState(state));
            printAction("Filling boiler...");
            coffeeMaker->fillBoiler([&printInfo](const string &boilerCapacityInfo) {
                printInfo(boilerCapacityInfo);
            });
            break;
        case CARAFE_FULL:
            printIssue(getReadableState(state));
            printAction("Emptying carafe...");
            coffeeMaker->emptyCarafe();
            break;
        case CARAFE_MISSING:
            printIssue(getReadableState(state));
            printAction("Replacing carafe...");
            coffeeMaker->replaceCarafe();
            break;
        case BOILER_COOL:
            printIssue(getReadableState(state));
            printAction("Boiling water..");
            coffeeMaker->startBoiling([&printInfo](const string &boilerHeaterInfo) {
                printInfo(boilerHeaterInfo);
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

void printPrefix(const string &message, const string &prefix, bool newline) {
    cout << "[" << prefix << "]: " << message;

    if (newline) {
        cout << endl;
    }
}