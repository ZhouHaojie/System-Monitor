#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream stream(kOSPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string unit;
  float mem_value;
  float mem_total;
  float mem_free;
  float mem_used;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> mem_value >> unit){
        if(key == "MemTotal:"){
          mem_total = mem_value;
        }
        else if(key == "MemFree:"){
          mem_free = mem_value;
        }
      }   
    }  
    mem_used = mem_total - mem_free;
  }
  return float(mem_used / mem_total);
}

// /proc/uptime: two values in seconds, the uptime of the system and the amount of time spent in the idle process
long LinuxParser::UpTime() { 
  string line;
  long up_time;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
  }
  return up_time; 
}


// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_list;
  string line;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      if(value != "cpu"){
        cpu_list.emplace_back(value);
      }
    }
  }
  return cpu_list;
}

// Read and return the number of jiffies for the system

long LinuxParser::Jiffies() { 
  // string line;
  // long jiffies;
  // long value;
  // string cpu;
  // std::ifstream stream(kProcDirectory + kStatFilename);
  // if(stream.is_open()){
  //   std::istringstream linestream(line);
  //   linestream >> cpu;
  //   //user + nice + system + idle + iowait + irq + softirq + steal
  //   for(int i=0; i<=7; i++){
  //     linestream >> value;
  //     jiffies += value;
  //   }  
  // }
  long jiffies;
  vector<string> cpu_list;
  for(auto value : cpu_list){
    jiffies += stol(value);
  }
   
  return jiffies; 
}

// Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  int token_id=0;
  long value;
  long active_jiffies;
  string kPidDirectory = "/" + to_string(pid);
  std::ifstream stream(kProcDirectory + kPidDirectory + kUptimeFilename);
  if(stream.is_open()){
    for(int i=1; i<=17; i++){
      std::istringstream linestream(line);
      if(token_id == CPUStates::kCstime || token_id == CPUStates::kCutime || token_id == CPUStates::kStime ||token_id ==CPUStates::kUtime){
        linestream >> value;
        active_jiffies += value;
      }
    }
    return active_jiffies;
  }
  return 0;  
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  // string line;
  // string cpu;
  // long idle_jiffies, user, nice, system, idle, iowait;
  // std::ifstream stream(kProcDirectory + kStatFilename);
  // if (stream.is_open()) {
  //   std::getline(stream, line);
  //   std::stringstream linestream(line);
  //   // idle + iowait
  //   linestream >> cpu >> user >> nice >> system >> idle >> iowait;
  //   idle_jiffies = idle + iowait;
  // }

  long idle_jiffies;
  vector<string> cpu_list = CpuUtilization();
  //idle + iowait
  idle_jiffies = stol(cpu_list[3]) + stol(cpu_list[4]);
  
  return idle_jiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return (Jiffies() - IdleJiffies());
}


// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int total_processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "processes") {
          linestream >> total_processes;
          break;
        }
      }
    }
  }
  return total_processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int procs_running;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "procs_running") {
          linestream >> procs_running;
          break;
        }
      }
    }
  }
  return procs_running;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  string cmd_line;
  string kPidDirectory = '/' + to_string(pid);
  std::ifstream stream(kProcDirectory + kPidDirectory + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmd_line;
  }
  return cmd_line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  long ram;
  string kPidDirectory = '/' + std::to_string(pid);
  std::ifstream stream(kProcDirectory + kPidDirectory + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") {
        linestream >> ram;
        break;
      }
    }
  }
  return to_string(ram / 1024); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string uid;
  string kPidDirectory = '/' + std::to_string(pid);
  std::ifstream stream(kProcDirectory + kPidDirectory + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "Uid:") {
          linestream >> uid;
          break;
        }
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string user;
  string uid;
  string placeholder;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> placeholder >> uid;
      if (uid == Uid(pid)) {
        break;
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string placeholder;
  long start_time;
  long Hertz;
  long seconds;
  string kPidDirectory = '/' + std::to_string(pid);
  std::ifstream stream(kProcDirectory + kPidDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int token_id = 1; token_id <= 22; ++token_id) {
      if (token_id == CPUStates::kStarttime) {
        linestream >> start_time;
      } 
      else {
        linestream >> placeholder;
      }
    }
  }
  Hertz = sysconf(_SC_CLK_TCK); //#include <unistd.h>
  seconds = start_time / Hertz;
  return seconds;
}