# Getting Started
- Editor
  - Install [VS Code](https://code.visualstudio.com/)
  - cd this directory
  - code .
  - Extensions (Ctrl+Shift+X)
    - C/C++
    - C/C++ Helper
    - CMake
    - C++ TestMate
    - Clang Format
    - Markdown All in One
  - Reload VsCode
- Compiler
  - Download [CMake](https://cmake.org/download/)
  - Ensure that you have a compiler (clang, gcc, msvc)
  - Update .vscode/tasks.json to generate files for your compiler
- Formatting
  - If you don't already have it, install [LLVM](https://llvm.org/releases/)
  - Required for clang-format to work
  - The VSCode keyboard shortcut for auto formatting is Ctrl+K Ctrl+D (may need to select the clang-formatter)
- Tasks.json
    - Update the proper build configuration to be default
- Run the program
    - Build the project (Ctrl+Shift+B)
    - launch.json has the specific launch commands

#Building From the Command Line
1. cd this directory
2. mkdir Build/VS/2019
3. mkdir Bin
4. cd Build/VS/2019
5. `cmake -G 'Visual Studio 16 2019' -DCMAKE_BUILD_TYPE=Debug ../../..`
6. `cmake --build . --config Release`


# How to create a new project
1. Create a new folder with the name of the project
2. Take the CMakeLists.txt file from an existing and copy it into the folder
   1. For a lib, look at Core
   2. For a test project, look at CoreTest
   3. For an exe, look at IncludeCounter
3. Open the CMakeLists file and change update the project name
4. Add any other libraries you need
   1. Something like `target_link_libraries(${PROJECT_NAME} Core)`
5. Add the inc and src folders to the root of your new library, next to the CMakeLists file
   1. For test project, add the Main.cpp and TestCommon.h files from another project
6. Open the root CMakeLists.txt file (next to this readme), and add an entry for your project
   1. `add_subdirectory(MyLibrary)`


# Useful Links
- [Gtest Documentation](https://github.com/google/googletest/blob/master/googletest/docs/primer.md)
- [Gmock Documentation](https://github.com/google/googletest/blob/master/googlemock/docs/cook_book.md)
- [Intro to CMake](https://www.johnlamp.net/cmake-tutorial-1-getting-started.html)
- [Markdown Syntax](https://www.markdownguide.org/basic-syntax)

# HELP, SOMETHING HAS GONE WRONG
Good luck with that...  
When you figure it out, make sure to update the README as necessary