cmake_minimum_required(VERSION 3.22.0)

project(slow5-wget)

# src
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/s5wget)
set(SOURCES src/main.c src/error.h src/s5wget/fetch.c src/s5wget/fetch.h src/s5wget/s5wget.c src/s5wget/s5wget.h)

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
