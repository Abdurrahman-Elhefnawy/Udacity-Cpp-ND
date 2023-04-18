#include "processor.h"
#include <vector>
#include <string>
#include <iostream>
#include "linux_parser.h"



// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {    
    std::vector<std::string> utili_vect = LinuxParser::CpuUtilization();

    int total_time{0};

    for(std::string val : utili_vect){
        total_time += std::stoi(val);
    }

    int idle_time = std::stoi(utili_vect[3]) + std::stoi(utili_vect[4]);
    
    
    float utilization = 1.0 * (total_time - idle_time) ;
    float usage = utilization / total_time;

  	return usage;

}