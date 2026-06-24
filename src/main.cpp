#include "process_manager.hpp"
#include <csignal>  // kill() function & SIGKILL signal macro
#include <cstdlib>  // system() function
#include <iostream> // std::cin & std::cout stream handling
#include <limits>   // data-type ranges (used for clearing input buffer)
#include <unistd.h> // usleep() & kill() functions

const std::string RESET = "\033[0m";
const std::string ORANGE = "\033[38;5;208m";
const std::string GREEN = "\033[32m";
const std::string BLUE = "\033[94m";
const std::string PALE_YELLOW = "\033[38;2;241;211;160m";
const std::string SLATE_BLUE = "\033[38;2;97;126;147m";

int main() {
  while (true) {
    system("clear");
    procList();

    std::string eofMessage = "\nEOF detected. Exiting Chansaw...\n";
    int choice;
    std::cout << SLATE_BLUE << "Enter a choice (" << PALE_YELLOW << "1"
              << SLATE_BLUE << ":" << PALE_YELLOW << "Refresh" << SLATE_BLUE
              << "," << PALE_YELLOW << "2" << SLATE_BLUE << ":" << PALE_YELLOW
              << "Kill Process" << SLATE_BLUE << "," << PALE_YELLOW << "3"
              << SLATE_BLUE << ":" << PALE_YELLOW << "Exit" << SLATE_BLUE
              << "): " << RESET;

    if (!(std::cin >> choice)) {
      if (std::cin.eof()) {
        std::cout << BLUE << eofMessage << RESET;
        break;
      }
      std::cout << ORANGE << "\nError: " << SLATE_BLUE
                << "Invalid input. Please enter a number.\n"
                << RESET;
      std::cin.clear();

      // limits lib used here
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      usleep(1500000);
      continue;
    }

    if (choice == 1) {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    } else if (choice == 2) {
      int targetPid;
      std::cout << SLATE_BLUE
                << "Select process PID you want to kill: " << RESET;

      if (!(std::cin >> targetPid)) {
        if (std::cin.eof()) {
          std::cout << BLUE << eofMessage << RESET;
          break;
        }
        std::cout << ORANGE << "\nError: " << SLATE_BLUE
                  << "Invalid input, Enter PID number.\n"
                  << RESET;
        std::cin.clear();
        // limits lib used here
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        usleep(1500000);
        continue;
      }

      // limits lib used here
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      if (kill(targetPid, SIGKILL) == 0) {
        std::cout << GREEN << "\nOK:" << SLATE_BLUE << " Process PID "
                  << PALE_YELLOW << targetPid << SLATE_BLUE
                  << " killed successfully.\n"
                  << RESET;
        usleep(1500000);
        continue;
      } else {
        perror("\nCan't kill the process: ");
        usleep(2500000);
        continue;
      }
    } else if (choice == 3) {
      // limits lib used here
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    } else {
      std::cout << ORANGE << "\nError: " << SLATE_BLUE
                << "Invalid option. Please choose" << PALE_YELLOW << "1"
                << SLATE_BLUE << "," << PALE_YELLOW << "2" << SLATE_BLUE
                << ",or" << PALE_YELLOW << "3" << SLATE_BLUE << "." << RESET;
      usleep(1500000);
      std::cin.clear();
      // limits lib used here
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return 0;
}
