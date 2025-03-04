set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(BINDING ${CMAKE_CURRENT_SOURCE_DIR}/bindings/c/tree_sitter)

add_custom_command(
    OUTPUT ${SRC}/parser.c ${SRC}/grammar.json ${SRC}/node-types.json ${BINDING}/tree-sitter-lumen.h
    COMMAND pnpm exec tree-sitter generate
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS grammar.js
    VERBATIM
)

add_library(tree-sitter-lumen ${SRC}/parser.c)
target_include_directories(tree-sitter-lumen PRIVATE ${SRC}/tree_sitter)
target_include_directories(tree-sitter-lumen PUBLIC ${BINDING})
