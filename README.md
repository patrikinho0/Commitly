# Commitly 🚀

**Commitly** is a lightweight desktop application that simplifies Git operations like staging, committing, and pushing — no command line needed. Designed for developers who want fast, intuitive version control right from a clean GUI.

---

## ✨ Features

- 📁 Add and stage files with one click  
- 📝 Write and commit changes easily  
- ⬆️ Push to remote repositories without terminal commands  
- 🔍 View current branch and status  
- 🧠 Smart use of `QProcess` to execute Git commands under the hood  
- 🔒 Safer, beginner-friendly alternative to direct CLI usage  

---

## 🧰 Built With

- **C++** – core logic and functionality  
- **Qt Creator** – full development environment  
- **Qt Framework** – UI and backend integration  
- **QProcess** – to handle Git operations (e.g. `git add`, `git commit`, `git push`)  
- **CMake / qmake** – project configuration and build

---

## 🚀 Installation

### Requirements
- **Git** installed and added to your system path  
- **Qt Creator** with Qt 5.x or 6.x  
- **C++17** compatible compiler  

### Steps
1. Clone this repository:
   ```bash
   git clone https://github.com/patrikinho0/Commitly.git
2. Open the .pro or CMakeLists.txt file in Qt Creator
3. Build and run the project
