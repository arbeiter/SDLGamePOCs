add_executable (clonegame
  main.cpp
  game.cpp
  game.h
)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIRS})

target_include_directories(clonegame PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(clonegame ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(clonegame SDL2::Image
  SDL2::Main)
