cmake_minimum_required(VERSION 3.10)
project(Simulation)

set(CMAKE_CXX_STANDARD 11)

include_directories("C:/Program Files/PostgreSQL/10/include/")

add_executable(Simulation main.cpp player.h player.cpp database.h database.cpp)

target_link_libraries(Simulation "C:/Program Files/PostgreSQL/10/lib/libpq.dll")
