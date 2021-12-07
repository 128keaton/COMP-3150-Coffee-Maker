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
     * Returns the current state
     * @return
     */
    T getState() {
        return this->currentState;
    }

    function<void(T)> stateFunction = nullptr;

private:
    T currentState;

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
