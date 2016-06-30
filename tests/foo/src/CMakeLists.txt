FILE(GLOB_RECURSE all_files
*.cpp
*.cc
*.c++
*.c
*.C)

include_directories(../../../../cub/include ../../../../event/include ../../../../trans-dsl/include ../../../include)

add_library(foo STATIC ${all_files})

