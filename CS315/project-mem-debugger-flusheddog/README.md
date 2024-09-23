# CS315 - Memory Debugger

### Project Documentation: 
This solution will be shared for both Project 4 and 5.
- [Project4](https://github.com/DigiPen-CS315/DigiPen-CS315/blob/main/projects/project-mem-debugger-windows/README.md) - Windows/MSVC Memory Debugger
- [Project5](https://github.com/DigiPen-CS315/DigiPen-CS315/blob/main/projects/project-mem-debugger-linux/README.md) - Linux/Clang Memory Debugger

### Compilers  
- Cover which compilers your application targets
    - [ ] gcc [version]  
    - [ ] clang [version]  
    - [ ] msvc/Visual Studio [version]  

### Integration  
- Overloaded the 10 new/delete functions
- Created Allocate/Deallocate functions and used VirualAlloc to allocate the extra page at the end to detect buffer overflow
- Added allocated memory to a vector to detect memory leaks when the program ends and detect delete errors 
- Logged memory leaks into a DebugLog.csv file

### Requirements  
- Add the MemDebugger.cpp and MemDebugger.h into the existing project

### Output  
- The file name, line number, address, amount of bytes, and function call of the leak are logged when a memory leak is detected
    - The file name of the log is DebugLog.csv and the header is Message, File, Line, Bytes, Address, Additional Info

---

### Continuous Integration Tests  
[![CMake_Windows_MSVC](https://github.com/DigiPen-CS315/project-mem-debugger-template/workflows/CMake_Windows/badge.svg)](https://github.com/DigiPen-CS315/project-mem-debugger-template/actions)  
[![CMake_Linux_Clang](https://github.com/DigiPen-CS315/project-mem-debugger-template/workflows/CMake_Linux/badge.svg?)](https://github.com/DigiPen-CS315/project-mem-debugger-template/actions)  
