add_executable (clonegame
  main.cpp
  game.cpp
  game.h
)

target_include_directories(clonegame PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(clonegame SDL2::Main)
