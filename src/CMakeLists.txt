cmake_minimum_required(VERSION 3.5)

project(dbcscanner-lib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dbcscanner-lib SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/codegen/c-main-generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codegen/c-util-generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codegen/conditional-tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codegen/c-sigprinter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codegen/filewriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codegen/fs-creator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/formatter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/dbclineparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/dbcscanner.cpp
)


target_compile_definitions(dbcscanner-lib PRIVATE DBCSCANNERLIB_LIBRARY)