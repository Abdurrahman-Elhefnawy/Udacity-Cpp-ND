#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
     int sec  = seconds%60;
     seconds /= 60;
     int min  = seconds % 60;
     int hour  = seconds / 60;
      
    string time_ = std::to_string(hour) + ":" + std::to_string(min) +":" + std::to_string(sec);

     return time_; 
}