#include <string>

#include "format.h"
#include <sstream>
#include <iomanip>
#include <math.h>
#include <iostream>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    long full_hours = seconds / 3600;
    long full_minutes = (seconds % 3600) / 60;
    long full_seconds = seconds % 60;

    std::stringstream ssfh;
    std::stringstream ssfm;
    std::stringstream ssfs;

    ssfh << std::setw(2) << std::setfill('0') << full_hours;
    ssfm << std::setw(2) << std::setfill('0') << full_minutes;
    ssfs << std::setw(2) << std::setfill('0') << full_seconds;

    return ssfh.str() + ":" + ssfm.str() + ":" + ssfs.str();
}