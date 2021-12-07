//
// Created by Keaton Burleson on 12/2/21.
//

#ifndef COFFEEMAKER_CARAFE_H
#define COFFEEMAKER_CARAFE_H

#include "../type/Tank.h"

class Carafe : private Tank{
public:
    explicit Carafe(double carafeCapacity = 100.0, double availableCoffee = 0.0): Tank(carafeCapacity, availableCoffee) {}

    double & getCurrentCapacity() {
        return this->currentCapacity;
    }

    double & getMaxCapacity() {
        return this->maxCapacity;
    }

    void replace() {
        this->available = true;
    }

    void remove() {
        this->available = false;
    }

    bool & isAvailable() {
        return this->available;
    }

    bool isEmpty()  {
        return this->currentCapacity == 0.0;
    }

    void empty() override {
        Tank::empty();
    }

    void brewFrom(Tank &other, double amount) {
        Tank::addFrom(other, amount);
    }

    void pour(double amount) {
        Tank::take(amount);
    }

private:
    bool available = true;
};

#endif //COFFEEMAKER_CARAFE_H
