cmake_minimum_required(VERSION 3.22.0)

project(slow5curl)

# src
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.c
    src/fetch.c
    src/fetch.h
    src/s5curl.c
    src/get.c
    src/index.c
    src/index.h
    include/slow5curl/s5curl.h
    )

add_executable(${PROJECT_NAME} ${SOURCES})

# slow5lib
include_directories(${CMAKE_SOURCE_DIR}/slow5lib/include)
include_directories(${CMAKE_SOURCE_DIR}/slow5lib/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/slow5lib)

target_link_libraries(${PROJECT_NAME} PUBLIC slow5 -lz -lcurl)
