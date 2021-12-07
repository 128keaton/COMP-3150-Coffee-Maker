//
// Created by Keaton Burleson on 12/7/21.
//

#ifndef COFFEEMAKER_HEATER_H
#define COFFEEMAKER_HEATER_H

#include "../sleep.h"
#include <string>
#include <functional>
#include <cmath>
#include "../helpers/StringFormatter.h"

using std::to_string, std::string, std::function;

struct Heater {
    bool isHeating = false;
    double temperatureValue = 0.0;
    double maxTemperature = 100.0;

    explicit Heater(double maxTemperature, double currentTemperature = 0.0) {
        this->maxTemperature = maxTemperature;
        this->temperatureValue = currentTemperature;
    }

    /**
    * Starts the heating process (calling a fake temperature generation function)
    * @param statusCallback - Lambda with a double value representing the temperature
    */
    void startHeating(const function<void(double)> &statusCallback) {
        double speed = this->maxTemperature / 10;
        double scaledTemperature = this->maxTemperature * 10;
        double finalScaledTemperature = scaledTemperature * 40;

        this->isHeating = true;

        for (unsigned t = 0; t <= finalScaledTemperature; t += (int) scaledTemperature) {
            // Calculate our temperature (takes our linear scale and creates a curve)
            double nextTemp = getEmulatedTemp((float) t, 0, (float) speed, (float) finalScaledTemperature);

            // Update the value
            this->temperatureValue = nextTemp;

            if (statusCallback != nullptr) {
                // Call the lambda function with our next temperature
                statusCallback(nextTemp);
            }


            // Sleep for "realness"
            millisleep(75 * (int)speed);
        }

        // Finally, set our heaterRelay to off (false)
        this->isHeating = false;

        // And set our heaterSensor to be reading the max temperature
        this->temperatureValue = this->maxTemperature;
    }

    [[nodiscard]] string getReadableStatus() const {
        return to_string_with_precision(this->temperatureValue, 2) + "C/" + to_string_with_precision(this->maxTemperature, 2) + "C - " +
               (this->isHeating ? "Heating" : "Not Heating");
    }

    void reset() {
        this->isHeating = false;
        this->temperatureValue = 0.0;
    }

    static double getEmulatedTemp(float current, float starting, float speed, float max) {
        return (-speed / 2 * (cos(M_PI * current / max) - 1) + starting) * 10;
    }
};

#endif //COFFEEMAKER_HEATER_H
