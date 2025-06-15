cmake_minimum_required(VERSION 3.10)
project(Compiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# ========== Tokenizer and others ==========
FLEX_TARGET(Lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

add_executable(table_parser
    ${FLEX_Lexer_OUTPUTS}
    src/table_parser.cpp
)

add_executable(recursive_parser
    ${FLEX_Lexer_OUTPUTS}
    src/recursive_parser.cpp
)

target_include_directories(table_parser PRIVATE include)
target_include_directories(recursive_parser PRIVATE include)

# ========== LR Parser ==========
FLEX_TARGET(LRLexer src/lr_lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lr_lexer.cpp)
BISON_TARGET(LRParser src/lr_parser.yacc ${CMAKE_CURRENT_BINARY_DIR}/lr_parser.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lr_parser.hpp)

add_executable(lr_parser
    ${FLEX_LRLexer_OUTPUTS}
    ${BISON_LRParser_OUTPUTS}
)

target_include_directories(lr_parser PRIVATE ${CMAKE_CURRENT_BINARY_DIR} include)
