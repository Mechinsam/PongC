cmake_minimum_required(VERSION 3.20)

project(pong LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# minifb
add_subdirectory(dependencies/minifb)

# proj stuff
file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

# Link MiniFB
target_link_libraries(${PROJECT_NAME} PRIVATE minifb)
