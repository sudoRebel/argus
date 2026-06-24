# Argus (CLI)

A lightweight, interactive, high-performance Linux Command-Line Task Manager written in standard C++.

---

# Features

* **Zero-Dependency**: Standard C++ library only.
* **Kernel Direct-Access**: Reads directly from the `/proc` filesystem.
* **PID Mapping**: Identifies active processes.
* **Interactive CLI Interface**: Real-time process monitoring with custom action loops (Refresh/Kill Porcess/Exit).
* **Process Management**: Send kernel signals (`SIGKILL`) to terminate any task instantly via PID.
* **Smart Memory Auto-Scaling**: Displays memory consumption dynamically in Kilobytes, Megabytes, Gigabytes, Terabytes, or Petabytes based on real usage & auto sorting.

---

# How it works

1. **Discovery**: Scans the `/proc` directory for PID-named folders using `dirent.h`.
2. **Filtering**: Uses `isdigit()` to isolate system processes.
3. **Extraction**: Parses `/proc/[PID]/status` to retrieve the process name and RAM usage (`VmRSS`).
4. **Sorting**: Sort lines by RAM usage (`VmRSS`) using `sort()` function.
5. **Interactivity**: Captures user input to safely dispatch POSIX signals.

---

# System Requirements

* **OS**: Linux-based operating systems.
* **Compiler**: Recommended Clang with C++17 support or higher.

---

# Installation & Run Guide

```bash
# 1. Clone the repository
git clone https://github.com/sudoRebel/argus.git && cd argus

# 2. Compile using Clang++ with C++17
clang++ -std=c++17 -I include/ src/*.cpp -o argus

# 3. Run the application with root privileges
sudo ./argus
