FILE(GLOB_RECURSE all_files
*.cpp
*.cc
*.c++
*.c
*.C)

include_directories(../../cub/include ../../event/include ../../trans-dsl/include)

add_library(l4 STATIC ${all_files})

install(TARGETS l4 ARCHIVE DESTINATION lib)

