//
// Created by Keaton Burleson on 12/6/21.
//

#ifndef COFFEEMAKER_PROGRESSBAR_H
#define COFFEEMAKER_PROGRESSBAR_H

#include <iostream>
#include <iomanip>
#include <string>


using std::cout,
std::flush,
std::string,
std::endl,
std::fixed,
std::setprecision;

class ProgressBar {
public:
    static void percentage(double current, double total = 100, const string &title = "", bool last = false) {
        double percentage = ((double) current) / ((double) total) * 100;

        if (percentage >= 0 && percentage <= 100) {
            string progress = "[" + string((int) percentage, '*') + string(100 - (int) percentage, ' ') + "]";

            cout << "\r\033[F" << progress << " " << (int) percentage << "% " << title << flush;
        }

        if (last) {
            cout << endl;
        }
    }

    static void temperature(double current, double max, const string &title = "", bool last = false) {
        double percentage = (current / max) * 100;
        string progress = "[" + string((int) percentage, '*') + string(100 - (int) percentage, ' ') + "]";
        cout <<
             "\r\033[F" <<
             progress <<
             " " <<
             fixed <<
             setprecision(2) <<
             current <<
             "°C" <<
             " / " <<
             fixed <<
             setprecision(0) <<
             max <<
             "°C " <<
             title <<
             flush;

        if (last) {
            cout << endl;
        }
    }
};

#endif //COFFEEMAKER_PROGRESSBAR_H
