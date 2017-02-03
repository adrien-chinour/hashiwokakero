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

#TESTS

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

##copy_game_bridges
add_executable(copy_game_bridges src/test/test_copy_game_bridges.c src/test/test_toolbox.c)
target_link_libraries(copy_game_bridges hashi)

##copy_game
add_executable(copy_game src/test/test_copy_game.c src/test/test_toolbox.c)
target_link_libraries(copy_game hashi)

##game_over
add_executable(game_over src/test/test_game_over.c src/test/test_toolbox.c)
target_link_libraries(game_over hashi)

##get_neighbour1
add_executable(get_neighbour1 src/test/test_get_neighbour1.c src/test/test_toolbox.c)
target_link_libraries(get_neighbour1 hashi)

##get_neighbour2
add_executable(get_neighbour2 src/test/test_get_neighbour2.c src/test/test_toolbox.c)
target_link_libraries(get_neighbour2 hashi)

##get_node_number1
add_executable(get_node_number1 src/test/test_get_node_number1.c src/test/test_toolbox.c)
target_link_libraries(get_node_number1 hashi)

##get_node_number2
add_executable(get_node_number2 src/test/test_get_node_number2.c src/test/test_toolbox.c)
target_link_libraries(get_node_number2 hashi)

##pont_equivalent
add_executable(pont_equivalent src/test/test_pont_equivalent.c src/test/test_toolbox.c)
target_link_libraries(pont_equivalent hashi)

##read_secure
add_executable(read_secure src/test/test_secure.c src/test/test_toolbox.c)
target_link_libraries(read_secure)

##testing
enable_testing()

add_test(can_add_bridges_dir1 can_add_bridges_dir1)
set_tests_properties (can_add_bridges_dir1 PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(can_add_bridges_dir2 can_add_bridges_dir2)
set_tests_properties (can_add_bridges_dir2 PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(can_add_bridges_dir3 can_add_bridges_dir3)
set_tests_properties (can_add_bridges_dir3 PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(can_add_bridges_dir4 can_add_bridges_dir4)
set_tests_properties (can_add_bridges_dir4 PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(can_add_bridges_dir5 can_add_bridges_dir5)
set_tests_properties (can_add_bridges_dir5 PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(copy_game copy_game)
set_tests_properties (copy_game PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(copy_game_bridges copy_game_bridges)
set_tests_properties (copy_game_bridges PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(game_over game_over)
set_tests_properties (game_over PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(get_neighbour1 get_neighbour1)
set_tests_properties (get_neighbour1 PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(get_neighbour2 get_neighbour2)
set_tests_properties (get_neighbour2 PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(get_node_number1 get_node_number1)
set_tests_properties (get_node_number1 PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(get_node_number2 get_node_number2)
set_tests_properties (get_node_number2 PROPERTIES PASS_REGULAR_EXPRESSION "1")

add_test(pont_equivalent pont_equivalent)
set_tests_properties (pont_equivalent PROPERTIES PASS_REGULAR_EXPRESSION "1")
