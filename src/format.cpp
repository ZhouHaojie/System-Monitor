#include <string>

#include "format.h"

using std::string;
using std::to_string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hour, min, second;
    hour = seconds / 3600;
    int temp = seconds % 3600;
    min = temp / 60;
    second = temp % 60;

    return to_string(hour) + ":" + to_string(min) + ":" + to_string(second);
}