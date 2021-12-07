//
// Created by Keaton Burleson on 12/7/21.
//

#ifndef COFFEEMAKER_STRINGFORMATTER_H
#define COFFEEMAKER_STRINGFORMATTER_H

#include <sstream>

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

#endif //COFFEEMAKER_STRINGFORMATTER_H
