#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
#include "processor.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid){}
  Process();
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process & a);  

  // Declare any necessary private members
 private:
 int pid_;
};

#endif