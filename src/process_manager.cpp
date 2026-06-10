#include "process_manager.h"
#include <algorithm> // sort() function
#include <dirent.h>  // for readding dirs
#include <fstream>
#include <iomanip> // std::fixed, std::setw, std::setprecision, std::left
#include <iostream>
#include <vector>

const std::string RESET = "\033[0m";
const std::string PALE_YELLOW = "\033[38;2;241;211;160m";
const std::string SLATE_BLUE = "\033[38;2;97;126;147m";

struct ProcessInfo {
  std::string pid;
  std::string procName;
  double vmRss;
};

void procList() {
  DIR *dir = opendir("/proc");

  if (dir == NULL) {
    perror("Could not open /proc");
    return;
  }

  std::vector<ProcessInfo> procsContainer;
  procsContainer.reserve(500);

  struct dirent *entry;

  while ((entry = readdir(dir)) != NULL) {
    if (isdigit(entry->d_name[0])) {
      std::string pid = entry->d_name;
      std::string statusFilePath = "/proc/" + pid + "/status";

      std::ifstream statusFile(statusFilePath);

      if (!statusFile.is_open()) {
        continue;
      }

      double vmRss = 0;
      std::string procName = "Unknown";
      std::string line;

      while (getline(statusFile, line)) {
        size_t charIndexForName = line.find("Name:");

        if (charIndexForName != std::string::npos) {
          std::string rawName = line.substr(charIndexForName + 5);
          size_t firstChar = rawName.find_first_not_of(" \t\r\n");
          size_t lastChar = rawName.find_last_not_of(" \t\r\n");

          if (firstChar != std::string::npos && lastChar != std::string::npos) {
            procName = rawName.substr(firstChar, (lastChar - firstChar + 1));
          }
        }

        size_t charIndexForVmRSS = line.find("VmRSS:");

        if (charIndexForVmRSS != std::string::npos) {
          std::string rawValue = line.substr(charIndexForVmRSS + 6);
          size_t firstChar = rawValue.find_first_not_of(" \t\r\n");
          size_t lastChar = rawValue.find_last_not_of(" \t\r\n");

          if (firstChar != std::string::npos && lastChar != std::string::npos) {
            try {
              vmRss =
                  stod(rawValue.substr(firstChar, (lastChar - firstChar + 1)));
            } catch (...) {
              vmRss = 0;
            }
          }
        }
      }
      statusFile.close();

      ProcessInfo proc;
      proc.pid = pid;
      proc.procName = procName;
      proc.vmRss = vmRss;

      procsContainer.push_back(proc);
    }
  }

  std::sort(procsContainer.begin(), procsContainer.end(),
            [](const ProcessInfo &a, const ProcessInfo &b) {
              if (a.vmRss != b.vmRss) {
                return a.vmRss > b.vmRss;
              }
              return a.procName < b.procName;
            });

  for (const auto &ref : procsContainer) {

    std::cout << std::left << SLATE_BLUE << "PID: " << PALE_YELLOW
              << std::setw(10) << ref.pid << SLATE_BLUE
              << " Name: " << PALE_YELLOW << std::setw(35) << ref.procName
              << SLATE_BLUE << " RAM: " << PALE_YELLOW;

    if (ref.vmRss >= 1099511627776.0) {
      std::cout << std::fixed << std::setprecision(2)
                << (ref.vmRss / 1099511627776.0) << " PB\n"
                << RESET;
    } else if (ref.vmRss >= 1073741824.0) {
      std::cout << std::fixed << std::setprecision(2)
                << (ref.vmRss / 1073741824.0) << " TB\n"
                << RESET;
    } else if (ref.vmRss >= 1048576.0) {
      std::cout << std::fixed << std::setprecision(2) << (ref.vmRss / 1048576.0)
                << " GB\n"
                << RESET;
    } else if (ref.vmRss >= 1024.0) {
      std::cout << std::fixed << std::setprecision(2) << (ref.vmRss / 1024.0)
                << " MB\n"
                << RESET;
    } else if (ref.vmRss >= 4.0) {
      std::cout << std::fixed << std::setprecision(2) << ref.vmRss << " kB\n"
                << RESET;
    } else {
      std::cout << "0 KB\n" << RESET;
    }
  }

  procsContainer.clear();
  closedir(dir);
}
