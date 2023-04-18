#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float MemTotal;
  float MemUsed;
  float MemFree;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = std::stof(value)/1000;
        }
        else if (key == "MemFree:") {
          MemFree = std::stof(value) /1000;
        }
      }
      MemUsed = MemTotal - MemFree;
    }
  }


  return ( (MemTotal-MemUsed) / MemTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line ;
  string uptime, ideltime;

  // std::cout<<kProcDirectory + kUptimeFilename;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  // std::cout<<"line";
  if (filestream.is_open()) {
     if (getline(filestream, line)) {
      std::istringstream linestream(line);
      // std::cout<<line;
      linestream>>uptime>>ideltime;
     }
     }
    
  return std::stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid ) {
  string line ;
  vector<std::string> data;
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatFilename);
  string value;
  long u_time ;
  long s_time ;
  long cu_time ;
  long cs_time ;

  if (filestream.is_open()) {
     while(getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
         data.push_back(value);
    	}
      u_time = std::stol(data[13]);
      s_time = std::stol(data[14]);
      cu_time =std::stol(data[15]);
      cs_time =std::stol(data[16]);
     }
  }
  long totalTime = u_time + s_time + cu_time +cs_time;
  return totalTime;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line ;
  std::vector<std::string> utiliz_vect;
  string cpu, user, nice, system, idle, iowait, irq, softirq;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
     if (getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream>>cpu>>user>>nice>>system>>idle>>iowait>>irq>>softirq;
      utiliz_vect.push_back(user);
      utiliz_vect.push_back(nice);
      utiliz_vect.push_back(system);
      utiliz_vect.push_back(idle);
      utiliz_vect.push_back(iowait);
      utiliz_vect.push_back(irq);
      utiliz_vect.push_back(softirq);
     }
     }
    else
      utiliz_vect.push_back("-1");
    
    // std::cout<<utiliz_vect[6];
    return utiliz_vect;
 
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {   
  string line ;
  
  string key, num_processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
     while(getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream>>key;
      if (key == "processes")
        linestream>>num_processes;
     }
    
     }
    
    return std::stoi(num_processes);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  string line ;
  
  string key, run_processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
     while(getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream>>key;
      if (key == "procs_running")
        linestream>>run_processes;
     }
     }
    return std::stoi(run_processes);
}


// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid){
  string line ;
  string cmd;
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kCmdlineFilename);

  if (filestream.is_open()) {
     while(getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream>>cmd;
     }
     }
    return cmd;   
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid){
  string line ;
  string key,ram;
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatusFilename);

  if (filestream.is_open()) {
     while(getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream>>key;
      if(key == "VmRSS:")
        linestream>>ram;
     }
     }

    return  to_string(stoi(ram) / 1024);   
}
// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid ) {
  string line ;
  string key,value;
  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatusFilename);

  if (filestream.is_open()) {
     while(getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream>>key;
      if(key == "Uid:")
        linestream>>value;
     }
     }

    return value; 
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid ) { 
  string line ;
  string user,x, r_uid;
  string uid = LinuxParser::Uid(pid);

  std::ifstream filestream(LinuxParser::kPasswordPath);

  if (filestream.is_open()) {
     while(getline(filestream, line)) {
      replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream>>user>>x>>r_uid;
      if(r_uid == uid)
        return user; 
     }
     }
  return "-1"; 
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid ) { 
  string line ;
  string uptime;

  std::ifstream filestream(kProcDirectory +std::to_string(pid)+ kStatFilename);

  if (filestream.is_open()) {
     while(getline(filestream, line)) {
      std::istringstream linestream(line);
      for(int i = 0 ; i<22; ++i)
        linestream>>uptime;
     }
     }

    
  return std::stoi(uptime) / sysconf(_SC_CLK_TCK);
}

float LinuxParser::CpuUtilization(int pid){

  long active_time = LinuxParser::ActiveJiffies(pid) / sysconf(_SC_CLK_TCK) ;
  long up_time  = LinuxParser::UpTime(pid);

  float utili = 1.0 * active_time / up_time;

  return  utili;   
}