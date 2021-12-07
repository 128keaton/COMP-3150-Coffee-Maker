//
// Created by Keaton Burleson on 12/6/21.
//

#ifndef COFFEEMAKER_HEATABLE_H
#define COFFEEMAKER_HEATABLE_H

#include <unistd.h>
#include <cmath>
#include <string>
#include "functional"
#include "Readable.h"
#include "StateChanges.h"
#include "../state/HeaterState.h"

using std::function, std::to_string, std::string;

class Heatable : public StateChanges<HeaterState> {
public:

    /**
     * Heatable type (Boiler, Hot Plate)
     * @param stateChanged - Callback Lambda for when the HeaterState has updated
     * @param maxTemperature - Max temperature this device can reach
     */
    explicit Heatable(double maxTemperature = 100) : StateChanges() {
        this->maxTemperature = maxTemperature;
        this->stateUpdated(
                {
                        false,
                        0.0
                }
        );

        this->heaterSensor.updated = [this](double temperature) {
            this->stateUpdated(
                    {
                            this->heaterRelay.get(),
                            temperature
                    }
            );
        };

        this->heaterRelay.updated = [this](bool isHeating) {
            this->stateUpdated(
                    {
                            isHeating,
                            this->heaterSensor.get()
                    }
            );
        };
    }

    /**
     * Starts the heating process (calling a fake temperature generation function)
     * @param statusCallback - Lambda with a double value representing the temperature
     */
    void startHeating(const function<void(double)> &statusCallback) {
        double speed = this->maxTemperature / 10;
        double scaledTemperature = this->maxTemperature * 10;
        double finalScaledTemperature = scaledTemperature * 40;

        for (unsigned t = 0; t <= finalScaledTemperature; t += (int) scaledTemperature) {
            // Calculate our temperature (takes our linear scale and creates a curve)
            double nextTemp = getEmulatedTemp((float) t, 0, (float) speed, (float) finalScaledTemperature);

            // Call the lambda function with our next temperature
            statusCallback(nextTemp);

            // Update the Readable value
            this->heaterSensor.set(nextTemp);

            // Sleep for "realness"
            usleep(15 * 5000 * (useconds_t) speed);
        }

        // Finally, set our heaterRelay to off (false)
        this->heaterRelay.set(false);

        // And set our heaterSensor to be reading the max temperature
        this->heaterSensor.set(this->maxTemperature);

        // Call our stateUpdated lambda with the new HeaterState
        this->stateUpdated(
                {
                        false,
                        this->maxTemperature
                }
        );
    }

    /**
     * Returns a string like "
     * @return
     */
    string readableHeaterStatus() {
        return to_string(this->heaterSensor.get()) + "/" + std::to_string(this->maxTemperature) + " - " +
               (this->heaterRelay.get() ? "Heating" : "Not Heating");
    }

    /**
     * Returns the current HeaterState
     * @return
     */
    HeaterState getHeaterState() {
        return this->getState();
    }

private:

    /**
     * Emulated temperature function based off of jQuery's easing functions.
     * Expected to be passed a double (current property) number that is scaling in a for-loop
     *
     * https://gist.github.com/Thyraz/e8327dbf4c34f8bd870d
     *
     * @param current - Current temperature value (scales linearly)
     * @param starting - Starting temperature (usually 0)
     * @param speed - Speed of the "curve" (10 in this case)
     * @param max - Maximum temperature
     * @return
     */
    static double getEmulatedTemp(float current, float starting, float speed, float max) {
        return (-speed / 2 * (cos(M_PI * current / max) - 1) + starting) * 10;
    }

    // Readable<double> sensor representing the temperature probe in the device
    Readable<double> heaterSensor = Readable<double>(0);

    // Readable<bool> relay representing the heater circuit relay in the device
    // Relays are typically used on high-amperage devices
    Readable<bool> heaterRelay = Readable<bool>(false);

    friend class Boiler;

private:
    double maxTemperature;
};

#endif //COFFEEMAKER_HEATABLE_H
