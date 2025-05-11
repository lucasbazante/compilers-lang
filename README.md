# A simple compiler

This repository contains the implementation of a simple compiler, built as a project for the Compilers class.

## Build project

With CMake, we do:
```sh
cmake -S . -B build
cmake --build build
```

## Running compiler
To run recursive parser, first create a sample code of the language in some `file.txt`, then run:

```sh
./build/recursive_parser < file.txt
```

To run the table parser, using the same sample code, run:

```sh
./build/table_parser < file.txt
```

## Examples
Some examples of programs can be found in `examples` folder. 
You can run the compilers in all examples with the `test` script

Usage:
```sh
./test 
```