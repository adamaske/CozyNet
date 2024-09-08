cmake_minimum_required(VERSION 3.16)

project(CozyNet VERSION 0.1 LANGUAGES CXX)

set(COZYNET_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include")
set(COZYNET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(COZYNET_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")

file(GLOB_RECURSE INCS "${COZYNET_INCLUDE_DIR}/*.h")
file(GLOB_RECURSE SRCS "${COZYNET_SOURCE_DIR}/*.cpp")

#config.h
configure_file("${COZYNET_RESOURCES_DIR}/Config.h.in" "${COZYNET_INCLUDE_DIR}/Config.h")

add_executable(${PROJECT_NAME} ${SRCS})

add_library(CozyNet_DIR STATIC ${SRCS})
target_include_directories(CozyNet_DIR PUBLIC ${COZYNET_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${COZYNET_INCLUDE_DIR})