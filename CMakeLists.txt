cmake_minimum_required(VERSION 3.22.0)

project(slow5curl)

# src
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/s5curl)
set(SOURCES
    src/main.c
    src/error.h
    src/s5curl/fetch.c
    src/s5curl/fetch.h
    src/s5curl/s5curl.c
    src/s5curl/s5curl.h
    src/s5curl/read.c
    src/s5curl/index.c
    src/s5curl/index.h
    )

add_executable(${PROJECT_NAME} ${SOURCES})

# slow5lib
include_directories(${CMAKE_SOURCE_DIR}/slow5lib/include)
include_directories(${CMAKE_SOURCE_DIR}/slow5lib/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/slow5lib)

target_link_libraries(${PROJECT_NAME} PUBLIC slow5 -lz -lcurl)
