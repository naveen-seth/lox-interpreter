add_library(token_types)
target_sources(token_types
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    token_types.cpp
)
target_compile_features(token_types PUBLIC cxx_std_20 PRIVATE cxx_std_23)

add_library(lexer)
target_sources(lexer
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    lexer.cpp
)
target_link_libraries(lexer PRIVATE token_types)

add_library(expr)
target_sources(expr
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    expr.cpp
)
target_compile_features(expr PUBLIC cxx_std_20 PRIVATE cxx_std_23)
target_link_libraries(expr PRIVATE lexer token_types)

add_library(interpreter)
target_sources(interpreter
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    interpreter.cpp
)
target_compile_features(interpreter PUBLIC cxx_std_20 PRIVATE cxx_std_23)
target_link_libraries(interpreter PRIVATE lexer expr)

add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_23)
target_link_libraries(main PRIVATE interpreter)

install(TARGETS main)
