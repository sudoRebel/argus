# ✨ Features
* **Zero-Dependency**: Standard C++ library only.
* **Kernel Direct-Access**: Reads directly from the `/proc` filesystem.
* **PID Mapping**: Identifies active processes.

# 💡 How it works
1. **Discovery**: Scans the `/proc` directory for PID-named folders using `dirent.h`.
2. **Filtering**: Uses `isdigit()` to isolate system processes.
3. **Extraction**: Parses `/proc/[PID]/status` to retrieve the process name and RAM usage.

# 🛠️ Installation & Run Guide
```bash
# Clone the repository
git clone https://github.com/sudoRebel/TaskManager

# Compile with G++
g++ -o TaskManager main.cpp

# Run the app
./TaskManager
