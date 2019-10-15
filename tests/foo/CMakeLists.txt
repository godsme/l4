cmake_minimum_required(VERSION 2.8)

project("foo")

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof  -g -std=c++11 -Wno-inconsistent-missing-override")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include" /usr/local/include)

add_subdirectory("src")

install(DIRECTORY include/foo DESTINATION include)

