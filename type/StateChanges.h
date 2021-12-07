//
// Created by Keaton Burleson on 12/2/21.
//

#ifndef COFFEEMAKER_STATECHANGES_H
#define COFFEEMAKER_STATECHANGES_H

#include "functional"

using std::function;

template<typename T>
class StateChanges {
public:

    /**
     * StateChanges constructor
     * @param stateChanged - A Lambda callback with the T value as the only parameter
     */
    explicit StateChanges(const function<void(T)> stateChanged) {
        this->stateFunction = stateChanged;
    }

    /**
     * Returns the current state
     * @return
     */
    T getState() {
        return this->currentState;
    }

private:
    T currentState;
    function<void(T)> stateFunction = nullptr;

protected:
    /**
     * Function called when the state is updated
     * @param state
     */
    void stateUpdated(T state) {
        this->currentState = state;
        if (this->stateFunction != nullptr) {
            this->stateFunction(state);
        }
    }
};
#endif //COFFEEMAKER_STATECHANGES_H
