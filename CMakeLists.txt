cmake_minimum_required(VERSION 3.28)

project(lumenlang)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/inc)

add_subdirectory(components/cpp-tree-sitter)
add_subdirectory(components/tree-sitter-lumen)

add_subdirectory(components/lumen-parser)
