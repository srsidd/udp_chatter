cmake_minimum_required(VERSION 2.8.3)
project(udp_chatter)

# Enable C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS program_options system thread)

# Include directories for building
include_directories(include ${catkin_INCLUDE_DIRS})

# Add UDPClient executable
add_executable(udp_chatter src/UDPClient.cpp)
target_link_libraries(udp_chatter ${Boost_LIBRARIES})

# Add UDPSender executable
add_executable(udp_sender src/UDPSender.cpp)
target_link_libraries(udp_sender ${Boost_LIBRARIES})
