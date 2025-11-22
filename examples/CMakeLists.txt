cmake_minimum_required(VERSION 3.20)

# Forklift example
add_executable(forklift_example forklift_example.cpp)
target_link_libraries(forklift_example roboos-sdk)

