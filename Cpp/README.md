# Getting Started
- Editor
  - Install [VS Code](https://code.visualstudio.com/)
  - cd this directory
  - code *.code-workspace
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

# Useful Links
- [Gtest Documentation](https://github.com/google/googletest/blob/master/googletest/docs/primer.md)
- [Gmock Documentation](https://github.com/google/googletest/blob/master/googlemock/docs/cook_book.md)
- [Intro to CMake](https://www.johnlamp.net/cmake-tutorial-1-getting-started.html)
- [Markdown Syntax](https://www.markdownguide.org/basic-syntax)

# HELP, SOMETHING HAS GONE WRONG
Good luck with that...  
When you figure it out, make sure to update the README as necessary