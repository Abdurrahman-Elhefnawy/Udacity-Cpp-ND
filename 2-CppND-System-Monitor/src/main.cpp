#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>
#include <string>
#include <vector>
#include "format.h"

int main() {
  // std::vector<std::string> temp = LinuxParser::CpuUtilization();
  // std::cout<<LinuxParser::CpuUtilization(9953);
  // format::ElapsedTime(6000) 
  System system;
  NCursesDisplay::Display(system);
}