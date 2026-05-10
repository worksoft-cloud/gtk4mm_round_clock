cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set some basic project attributes
project (round_clock)
#	VERSION 0.1
#	DESCRIPTION "A Hello World Project")

# Enable pkg-config
find_package(PkgConfig REQUIRED)

# Check for gtk4
#pkg_check_modules(GTK REQUIRED gtk4)

# Check for gtkmm-4.0
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

# Check for cairomm-1.16
pkg_check_modules(CAIROMM REQUIRED cairomm-1.16)

# Add include directories
include_directories(
    ${GTKMM_INCLUDE_DIRS}
    ${CAIROMM_INCLUDE_DIRS}
#    ${GTK_INCLUDE_DIRS}
)

# Add compiler flags
add_definitions(
    ${GTKMM_CFLAGS_OTHER}
    ${CAIROMM_CFLAGS_OTHER}
#    ${GTK_CFLAGS_OTHER}
)

# This project will output an executable file
add_executable(${PROJECT_NAME} round_clock.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${GTKMM_LIBRARIES}
    ${CAIROMM_LIBRARIES}
#    ${GTK_LIBRARIES}
)

# Create a simple configuration header
#configure_file(config.h.in config.h)

# Include the configuration header in the build
#target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
