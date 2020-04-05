#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

  // Declare any necessary private members
 private:
 long preTotal = 0;
 long preIdle = 0;  // how to define?
};

#endif