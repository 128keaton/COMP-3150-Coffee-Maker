//
// Created by Keaton Burleson on 12/1/21.
//

#ifndef COFFEEMAKER_READABLE_H
#define COFFEEMAKER_READABLE_H
#include "functional"
#include <memory>
#include <utility>

using std::function;

template<typename T>
class Readable {
public:

    function<void(T)> updated = nullptr;

    /**
     * Readable constructor
     * @param initialValue - T value representing the initial value of T
     */
    explicit Readable(T initialValue) {
        this->value = initialValue;
        this->set(initialValue);
    }

    /**
     * Returns the value of type T
     * @return (value)
     */
    T& get() {
        return (this->value);
    }

    /**
     * Update the value of type T, which triggers a callback
     * @param newValue - T newValue
     */
    void set(const T newValue) {
        this->value = newValue;

        if (this->updated != nullptr) {
            this->updated(newValue);
        }
    }

private:
    T value;
};

#endif //COFFEEMAKER_READABLE_H
