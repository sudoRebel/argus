#include "process_manager.h"
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

void procList() {
  DIR *dir = opendir("/proc");

  if (dir == NULL) {
    perror("Could not open /proc");
    return;
  }

  struct dirent *entry;
  std::string pid, statusFilePath, word, vmRss, procName;
  double kiloByte, bytes, megaByte, gigaByte, teraByte, petaByte;

  while ((entry = readdir(dir)) != NULL) {
    if (isdigit(entry->d_name[0])) {
      pid = entry->d_name;
      statusFilePath = "/proc/" + pid + "/status";

      std::ifstream statusFile(statusFilePath);
      if (!statusFile.is_open())
        continue;

      vmRss = "0";
      procName = "Unknown";

      while (statusFile >> word) {
        if (word == "Name:") {
          statusFile >> procName;
        } else if (word == "VmRSS:") {
          statusFile >> vmRss;
          break;
        }
      }

      try {
        kiloByte = stod(vmRss);
      } catch (const std::invalid_argument &e) {
        kiloByte = 0;
      }

      std::cout << std::left << "PID: " << std::setw(10) << pid
                << " Name: " << std::setw(35) << procName << " RAM: ";

      if (kiloByte >= 1099511627776.0) {
        petaByte = kiloByte / 1099511627776.0;
        std::cout << petaByte << " PB\n";
      } else if (kiloByte >= 1073741824.0) {
        teraByte = kiloByte / 1073741824.0;
        std::cout << teraByte << " TB\n";
      } else if (kiloByte >= 1048576.0) {
        gigaByte = kiloByte / 1048576.0;
        std::cout << gigaByte << " GB\n";
      } else if (kiloByte >= 1024.0) {
        megaByte = kiloByte / 1024.0;
        std::cout << megaByte << " MB\n";
      } else if (kiloByte >= 1.0) {
        std::cout << kiloByte << " kB\n";
      } else {
        bytes = kiloByte * 1024.0;
        std::cout << bytes << " Byte\n";
      }

      statusFile.close();
    }
  }
  closedir(dir);
}
