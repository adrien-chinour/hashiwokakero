#version minimale de cmake nécessaire
cmake_minimum_required(VERSION 3.3)

#nom du projet
project (hashi-tm2h C)

#executable interface text
add_executable(hashi_text src/text/hashi_text.c)
target_link_libraries(hashi_text hashi text)

# libhashi
add_library(hashi src/core/game.c src/core/node.c)

#libtext
add_library(text src/core/generate.c src/text/interface.c)

#tests

##can_add_bridges_dir1
add_executable(can_add_bridges_dir1 src/test/test_can_add_bridge_dir1.c src/test/test_toolbox.c)
target_link_libraries(can_add_bridges_dir1 hashi)

##can_add_bridges_dir2
add_executable(can_add_bridges_dir2 src/test/test_can_add_bridge_dir2.c src/test/test_toolbox.c)
target_link_libraries(can_add_bridges_dir2 hashi)

##can_add_bridges_dir3
add_executable(can_add_bridges_dir3 src/test/test_can_add_bridge_dir3.c src/test/test_toolbox.c)
target_link_libraries(can_add_bridges_dir3 hashi)

##can_add_bridges_dir4
add_executable(can_add_bridges_dir4 src/test/test_can_add_bridge_dir4.c src/test/test_toolbox.c)
target_link_libraries(can_add_bridges_dir4 hashi)

##can_add_bridges_dir5
add_executable(can_add_bridges_dir5 src/test/test_can_add_bridge_dir5.c src/test/test_toolbox.c)
target_link_libraries(can_add_bridges_dir5 hashi)

##testing
enable_testing()

add_test(can_add_bridges_dir1 can_add_bridges_dir1)
set_tests_properties (can_add_bridges_dir1 PROPERTIES PASS_REGULAR_EXPRESSION "1")