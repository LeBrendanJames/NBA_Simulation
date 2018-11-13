cmake_minimum_required(VERSION 3.10)
project(Simulation)

set(CMAKE_CXX_STANDARD 11)

include_directories("C:/Program Files/PostgreSQL/10/include/")

add_executable(Simulation main.cpp player.h player.cpp Database/DBInterface.h Database/DBInterface.cpp Database/DBReq.h Database/DBReq.cpp Database/Query.cpp Database/Query.h Database/DBRes.cpp Database/DBRes.h AdvStats.h AdvStats.cpp)

target_link_libraries(Simulation "C:/Program Files/PostgreSQL/10/lib/libpq.dll")
