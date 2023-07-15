cmake_minimum_required(VERSION 3.22.0)

project(slow5-wget)

# src
include_directories(${CMAKE_SOURCE_DIR}/src)
set(SOURCES src/main.c src/error.h src/fetch.c src/fetch.h src/slow5_wget.c src/slow5_wget.h)

add_executable(${PROJECT_NAME} ${SOURCES})

# slow5lib
include_directories(${CMAKE_SOURCE_DIR}/slow5lib/include)
include_directories(${CMAKE_SOURCE_DIR}/slow5lib/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/slow5lib)

target_link_libraries(${PROJECT_NAME} PUBLIC slow5 -lz)

# curl
include_directories(${CMAKE_SOURCE_DIR}/curl/include)
include_directories(${CMAKE_SOURCE_DIR}/curl/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/curl)

target_link_libraries(${PROJECT_NAME} PUBLIC libcurl)
