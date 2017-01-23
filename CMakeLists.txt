#version minimale de cmake nécessaire
cmake_minimum_required(VERSION 2.6)

#nom du projet
project (hashi-tm2h C)

add_executable(hashi_text src/hashi_text.c)
target_link_libraries(hashi_text hashi text)

add_library(hashi src/game.c src/node.c)

add_library(text src/generate.c src/interface.c)

add_executable(test tests/test_game1.c tests/test_toolbox.c)
target_link_libraries(test hashi)

enable_testing()
add_test(tg1 test)