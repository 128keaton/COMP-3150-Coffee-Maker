//
// Created by Keaton Burleson on 12/7/21.
//

#ifndef COFFEEMAKER_TANK_H
#define COFFEEMAKER_TANK_H

#include <unistd.h>
#include <string>
#include <functional>
#include "../helpers/StringFormatter.h"

using std::string, std::function;

struct Tank {

protected:
    double currentCapacity = 0.0;
    double maxCapacity = 100.0;

public:
    explicit Tank(double maxCapacity, double currentCapacity = 0.0) {
        this->maxCapacity = maxCapacity;
        this->currentCapacity = currentCapacity;
    }

    virtual /**
    * Empty the tank
    */
    void empty() {
        this->currentCapacity = 0.0;
    }

    /**
     * Add a specified amount of liquid to this Tank
     * Will not go over max capacity
     *
     * @param amount - double, defaults to 0.0
     */
    void add(double amount = 0.0) {
        if ((this->currentCapacity + amount) <= this->maxCapacity) {
            this->currentCapacity += amount;
        } else {
            this->currentCapacity = this->maxCapacity;
        }
    }

    /**
     * Add a specified amount of liquid to this Tank from another Tank
     * @param other - Other Tank to take liquid from
     * @param amount - Amount of liquid to take from other and put into this Tank, defaults to 10.0
     */
    void addFrom(Tank &other, double amount = 10.0) {
        this->add(other.take(amount));
    }

    /**
     * Take a specified amount of liquid from this Tank
     * Will return 0 if the amount is greater than what is available
     *
     * @param amount - Amount of liquid to take, defaults to 10.0
     * @return
    */
    double take(double amount = 10.0) {
        if (this->currentCapacity >= amount) {
            this->currentCapacity -= amount;
            return amount;
        }

        return 0;
    }

    virtual
    /**
     * Fill this Tank (in an emulated way)
     * @param statusCallback - Lambda with a double parameter representing the current fluid level
     * @param fillSpeed - How fast you want to fill this bad boy up, defaults to 10
     */
    void fill(const function<void(double)> &statusCallback, int fillSpeed) {
        for (int amount = (int) this->currentCapacity; amount <= this->maxCapacity; amount += fillSpeed) {
            this->currentCapacity = amount;
            statusCallback(amount);
            usleep(15 * 5000 * 10);
        }

        this->currentCapacity = this->maxCapacity;
    }

    /**
    * Returns a string like (currentCapacity)/(maxCapacity)
    * @return
    */
    [[nodiscard]] string getReadableStatus() const {
        return to_string_with_precision(this->currentCapacity, 2) + "mL/" + to_string_with_precision(this->maxCapacity, 2) + "mL";
    }

};

#endif //COFFEEMAKER_TANK_H
