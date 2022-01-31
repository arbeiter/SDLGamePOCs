cmake_minimum_required(VERSION 3.13)

project(SDL2Test)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_subdirectory(clonegame)

set(APPLICATION_NAME ${PROJECT_NAME})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
file(COPY "${PROJECT_SOURCE_DIR}/res" DESTINATION "${PROJECT_SOURCE_DIR}/bin/clonegame")
