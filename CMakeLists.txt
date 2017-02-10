cmake_minimum_required(VERSION 2.6)
project("hashi-tm2h" LANGUAGES C)

file( GLOB_RECURSE text_src_files src/text/* ) # adding files for text_src_files
file( GLOB_RECURSE gui_src_files src/gui/* ) # adding files for gui_src_files

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(hashi_text ${text_src_files}) # hashi_text
add_executable(hashi_gui ${gui_src_files}) # hashi_gui

add_library(hashi src/core/game.c src/core/node.c) # libhashi
add_library(hashi-tools src/core/file.c src/core/secure.c) # libhashi-tools

# linking libraries for hashi_text
target_link_libraries(hashi_text
  hashi
  hashi-tools)

# linking libraries for hashi_gui
target_link_libraries(hashi_gui
  hashi
  hashi-tools
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${SDL2_TTF_LIBRARY}) 


