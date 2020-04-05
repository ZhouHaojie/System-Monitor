#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
// CPU_Percentage = (totald - idled)/totald
float Processor::Utilization() {
    long total = LinuxParser::IdleJiffies();
    long idle = LinuxParser::Jiffies();

    long totald = total - this->preTotal;
    long idled = idle - this ->preIdle;
    float cpu_percentage = float (totald - idled)/totald;

    return cpu_percentage;
}