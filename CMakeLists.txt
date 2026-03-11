cmake_minimum_required(VERSION 3.10)

project(ExprParser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    scanner.cpp
    parser.cpp
)

set(HEADERS
    scanner.h
    parser.h
)

add_executable(expr_parser ${SOURCES} ${HEADERS})

target_include_directories(expr_parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})