# A simple compiler

This repository contains the implementation of a simple compiler, built as a project for the Compilers (DIM0164) class.

## Building the project

With CMake, from the root of the project, we do:
```sh
cmake -S . -B build
cmake --build build
```

## Running the compiler
If you want to run/inspect the descendent syntactic analysis, please refer to the [alternative branch/version][https://github.com/lucasbazante/compilers-lang/tree/descendent_syntactic_analysis] of this project and follow the instructions.

To run the Bison parser, assuming that `file` is a sample code of the parsed language, simply do:

```sh
./build/parser < file > output_file_name.cpp
```

The output file needs to be in C++ because the internal mechanisms of the code (such as I/O) are implemented in C++.
After this, just compile the file with `g++` and run it as a normal program.

## Examples
Some examples of programs can be found in `examples` folder.
