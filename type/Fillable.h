//
// Created by Keaton Burleson on 12/1/21.
//

#ifndef COFFEEMAKER_FILLABLE_H
#define COFFEEMAKER_FILLABLE_H

#include <unistd.h>
#include <string>
#include "Readable.h"

using std::to_string, std::string;

class Fillable : public Readable<double> {
public:

    /**
     * Fillable constructor
     * @param maxCapacity - Double value representing the max capacity of this Fillable
     * @param updated - Lambda callback with a double parameter representing the liquid level
     */
    explicit Fillable(double maxCapacity, const function<void(double)> &updated) : Readable(0.0, updated) {
        this->maxCapacity = maxCapacity;
        this->empty();
    }

    /**
     * Empty the Fillable
     */
    void empty() {
        this->set(0.0);
    }

    /**
     * Add a specified amount of liquid to this Fillable
     * Will not go over max capacity
     *
     * @param amount - double, defaults to 0.0
     */
    void add(double amount = 0.0) {
        double currentAmount = this->get();

        if (currentAmount + amount <= this->maxCapacity) {
            this->set(amount + currentAmount);
        } else {
            this->set(this->maxCapacity);
        }
    }

    /**
     * Add a specified amount of liquid to this Fillable from another Fillable
     * @param other - Other Fillable to take liquid from
     * @param amount - Amount of liquid to take from other and put into this Fillable, defaults to 10.0
     */
    void addFrom(Fillable &other, double amount = 10.0) {
        double otherValue = other.get();

        this->add(amount);
        other.set(otherValue - amount);
    }

    /**
     * Take a specified amount of liquid from this Fillable
     * Will return 0 if the amount is greater than what is available
     *
     * @param amount - Amount of liquid to take, defaults to 10.0
     * @return
     */
    double take(double amount = 10.0) {
        double currentAmount = this->get();

        if (currentAmount >= amount) {
            this->set(currentAmount - amount);
            return amount;
        }

        return 0;
    }

    virtual /**
     * Fill this Fillable (in an emulated way)
     * @param statusCallback - Lambda with a double parameter representing the current fluid level
     * @param fillSpeed - How fast you want to fill this bad boy up, defaults to 10
     */
    void fill(const function<void(double)> &statusCallback, int fillSpeed = 10) {
        for (int amount = (int) this->get(); amount <= this->maxCapacity; amount += fillSpeed) {
            this->set(amount);
            statusCallback(amount);
            usleep(15 * 5000 * 10);
        }

        this->set(this->maxCapacity);
    }

    /**
     * Returns a string like (currentCapacity)/(maxCapacity)
     * @return
     */
    string readableFillStatus() {
       return to_string(this->get()) + "/" + to_string(this->getMaxCapacity());
    }

    /**
     * Returns the max capacity of this Fillable
     * @return
     */
    [[nodiscard]] double getMaxCapacity() const {
        return this->maxCapacity;
    }

private:
    double maxCapacity;
};

#endif //COFFEEMAKER_FILLABLE_H
