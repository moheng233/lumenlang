include(FetchContent)

FetchContent_Declare(
    tree-sitter
    GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter.git
    GIT_TAG v0.25.2
)

FetchContent_MakeAvailable(tree-sitter)

FetchContent_GetProperties(
    tree-sitter
    SOURCE_DIR FETCHCONTENT_SOURCE_DIR_TREE-SITTER
)

add_library(
    tree-sitter
    ${FETCHCONTENT_SOURCE_DIR_TREE-SITTER}/lib/src/lib.c
)

target_include_directories(tree-sitter PRIVATE ${FETCHCONTENT_SOURCE_DIR_TREE-SITTER}/lib/src)
target_include_directories(tree-sitter PUBLIC ${FETCHCONTENT_SOURCE_DIR_TREE-SITTER}/lib/include)

add_library(cpp-tree-sitter INTERFACE)

target_include_directories(cpp-tree-sitter INTERFACE inc)
target_link_libraries(cpp-tree-sitter INTERFACE tree-sitter)
