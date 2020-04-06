#include <string>

#include "format.h"

using std::string;
using std::to_string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hour, minute, second;
    string hh, mm, ss;
    hour = seconds / 3600;
    minute = (seconds % 3600) / 60;
    second = (seconds % 3600) % 60;
    hh = (hour < 10) ? "0" + to_string(hour): to_string(hour);
    mm = (minute < 10) ? "0" + to_string(minute): to_string(minute);
    ss =  (second < 10) ? "0" + to_string(second): to_string(second);
    return string(hh + ":" + mm + ":" +ss); 
}