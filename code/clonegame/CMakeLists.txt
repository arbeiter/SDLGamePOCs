add_executable (clonegame
  main.cpp
  game.cpp
  game.h
  player.h
  player.cpp
  textureManager.h
  textureManager.cpp
)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIRS})
include_directories(${SDL2TTF_INCLUDE_DIR})

target_include_directories(clonegame PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(clonegame ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY})
target_link_libraries(clonegame SDL2::Image SDL2::Main SDL2::TTF)
file(COPY "${PROJECT_SOURCE_DIR}/res" DESTINATION "${PROJECT_SOURCE_DIR}/bin/clonegame")
