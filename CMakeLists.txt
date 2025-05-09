cmake_minimum_required(VERSION 3.10)
project(Compiler)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include headers from include/
include_directories(include)

# Find Flex and generate lexer.cpp
find_package(FLEX REQUIRED)
FLEX_TARGET(Lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

add_executable(tokenizer_test
    ${FLEX_Lexer_OUTPUTS}
    src/tokenizer_test.cpp
)

add_executable(table_parser
    ${FLEX_Lexer_OUTPUTS}
    src/parser.cpp
)

add_executable(recursive_parser
    ${FLEX_Lexer_OUTPUTS}
    src/recursive_parser.cpp
)

# Include directory for both targets
target_include_directories(tokenizer_test PRIVATE include)
target_include_directories(table_parser PRIVATE include)
