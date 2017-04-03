cmake_minimum_required(VERSION 2.6)
project("hashi-tm2h" LANGUAGES C)
set(CMAKE_C_COMPILER gcc)

# Ajout des fichiers .c pour les executables
file( GLOB_RECURSE text_src_files src/text/* ) # adding files for text_src_files
file( GLOB_RECURSE gui_src_files src/gui/* ) # adding files for gui_src_files
file( GLOB_RECURSE solve_src_files src/solver/* )

#Ajout des modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}) # pour trouver le repertoire sans SDL2

# creation des librairies et executables
add_executable(hashi_text ${text_src_files}) # hashi_text
add_executable(hashi_gui ${gui_src_files}) # hashi_gui
add_executable(hashi_solve ${solve_src_files}) #hashi_solve
add_library(hashi src/core/game.c src/core/node.c) # libhashi
add_library(hashi-tools src/tools/file.c
												src/tools/secure.c
												src/tools/generate.c
												src/solver/solver.c) # libhashi-tools

#Pour installer les executables à la racine
install(TARGETS hashi_gui hashi_text hashi_solve DESTINATION ${PROJECT_SOURCE_DIR})

# optin de debugage
option(ENABLE_DEBUG "debug options" ON)
if(ENABLE_DEBUG)
  set(CMAKE_C_FLAGS "-std=c99 -g -fprofile-arcs -ftest-coverage -Wall")
  set(CMAKE_LD_FLAGS "-fprofile-arcs -ftest-coverage")
  INCLUDE(CTest)
  INCLUDE(Dart)
else()
  set(CMAKE_C_FLAGS "-std=c99 -O3 -Wall")
endif()

# linking library hashi for library hashi-tools
target_link_libraries(hashi-tools hashi)

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

# linking libraries for hashi_solve
target_link_libraries(hashi_solve
  hashi
  hashi-tools)
