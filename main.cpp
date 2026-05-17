#include <cctype>
#include <dirent.h>
#include <fstream>
#include <iostream>

using namespace std;

void procList() {
  DIR *dir = opendir("/proc/");

  if (dir == NULL) {
    perror("Could not open /proc");
    return;
  }

  struct dirent *entry;
  string word;

  while ((entry = readdir(dir)) != NULL) {
    if (isdigit(entry->d_name[0])) {
      string pid = entry->d_name;
      string statusFilePath = "/proc/" + pid + "/status";

      ifstream statusFile(statusFilePath);
      if (!statusFile.is_open())
        continue;

      string VmRSS = "0";
      string procName = "Unknown";

      while (statusFile >> word) {
        if (word == "Name:") {
          statusFile >> procName;
        } else if (word == "VmRSS:") {
          statusFile >> VmRSS;
          break;
        }
      }

      cout << "PID: " << pid << " \t Name: " << procName << " \t RAM: " << VmRSS
           << " kB" << endl;

      statusFile.close();
    }
  }
  closedir(dir);
}

int main() {
  procList();
  return 0;
}
