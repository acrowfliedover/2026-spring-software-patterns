# /buildandrun

Build the project and run the executable.

## Steps

1. **Build** from the project root using the CMake + Makefile flow (see `.cursor/rules/cmake-build.mdc`):
   - **Windows (PowerShell):**  
     `cd build; cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++.exe ../src; cmake --build .`
   - **Unix-like:**  
     `mkdir -p build && cd build && cmake ../src && cmake --build .`

2. **Run** the `Patterns` executable from the `build/` directory:
   - **Windows:** `.\Patterns.exe t ..\Test2.xml` (tokenizer test on Test2.xml)
   - **Unix-like:** `./Patterns t ../Test2.xml`
   - If the user provided extra arguments (e.g. a different mode or file), use those instead: `Patterns t|s|v [file(s)]`.

Execute the build and run commands in the terminal and show the user the output.
