#include <dirent.h>
#include <unistd.h>
#include <sstream>
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
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  string line;
  string key, memUnit;
  int value;
  int memTotal, memFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) 
  {
    for (int i = 0; i < 2; i++)
    {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      while (linestream >> key >> value >> memUnit)
      {
        if (key == "MemTotal:")
          memTotal = value;
        else if (key == "MemFree:")
          memFree = value;
      }
    }
  }
  return (float)((memTotal - memFree) / memTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string line;
  float val1, val2;
  long upTime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
   	std::istringstream linestream(line);
    linestream >> val1 >> val2;
    upTime = (long)(val1 + val2);
    return upTime;
  }
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  string line;
  long val14, val15, val16, val17;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open())
  {
    for (int i = 0; i < 13; i++) 
    {
      filestream.ignore(256, ' ');
    }
    
    filestream >> val14 >> val15 >> val16 >> val17;
    return (val14 + val15 + val16 + val17);
  }
  return (val14 + val15 + val16 + val17);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{ 
  string line;
  string key;
  int activeJiffies;
  int val1, val2, val3, val4, val5, val6, val7, val8, val9, val10;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> val1 >> val2 >> val3 >> val4 >> val5 >> val6 >> val7 >> val8 >> val9 >> val10;
    activeJiffies = val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10;
    return (long)activeJiffies;
  }
  return (long)activeJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  string line;
  string key;
  string user, nice, system, idle, iowait;
  long idleJiffies;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait;
    //linestream >> key >> user >> nice, system, idle, iowait;
    idleJiffies = std::stol(idle) + std::stol(iowait);
    return idleJiffies;
  }
  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  vector<string> jiffies;
  string line;
  string key;
  int val1, val2, val3, val4, val5, val6, val7, val8, val9, val10;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> val1 >> val2 >> val3 >> val4 >> val5 >> val6 >> val7 >> val8 >> val9 >> val10;
    jiffies.push_back(std::to_string(val1));
    jiffies.push_back(std::to_string(val2));
    jiffies.push_back(std::to_string(val3));
    jiffies.push_back(std::to_string(val4));
    jiffies.push_back(std::to_string(val5));
    jiffies.push_back(std::to_string(val6));
    jiffies.push_back(std::to_string(val7));
    jiffies.push_back(std::to_string(val8));
    jiffies.push_back(std::to_string(val9));
    jiffies.push_back(std::to_string(val10));
  }
  return jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "processes")
        {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "procs_running")
          return value;
      }
    }
  }
  return value; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) 
  {
      std::getline(filestream, line);
      return line;
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return to_string(value / 1024);
        }
      }
    }
  }
  return to_string(value / 1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return to_string(value);
        }
      }
    }
  }
  return to_string(value);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{
  string line;
  string username, x;
  int uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kPasswordPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ":", " ");
      std::istringstream linestream(line);
      linestream >> username >> x >> uid;
      if (to_string(uid) == Uid(pid))
        return username;
    }
  }
  return username;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{
  string token;
  long upTime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kUptimeFilename);
  if (filestream.is_open())
  {
    for (int i = 0; i < 22; i++)
    {
      filestream >> token;
    }
    upTime = std::stol(token) / sysconf(_SC_CLK_TCK);
    return upTime;
  }
  return upTime;
}
