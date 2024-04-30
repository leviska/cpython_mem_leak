cmake_minimum_required(VERSION 3.29)
project(MemLeak)

# Find Python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

if (Python3_FOUND)
    message(STATUS "Python found: ${Python3_VERSION}")
    message(STATUS "Python executable: ${Python3_EXECUTABLE}")
    message(STATUS "Python include directories: ${Python3_INCLUDE_DIRS}")
    message(STATUS "Python libraries: ${Python3_LIBRARIES}")
else()
    message(FATAL_ERROR "Python not found!")
endif()

# Add executable
add_executable(main main.cpp)

# Link Python libraries
target_link_libraries(main PRIVATE Python3::Python)
