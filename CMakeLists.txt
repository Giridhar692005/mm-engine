cmake_minimum_required(VERSION 3.20)
project(mm_engine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

add_library(mm_core src/order_book.cpp)
target_include_directories(mm_core PUBLIC include)

enable_testing()
add_executable(test_order_book tests/test_order_book.cpp)
target_link_libraries(test_order_book mm_core)
add_test(NAME order_book COMMAND test_order_book)
add_executable(run_sim src/order_generator.cpp)

target_link_libraries(run_sim mm_core)
add_library(mm_core src/order_book.cpp src/agent.cpp)
target_include_directories(mm_core PUBLIC include)

enable_testing()
add_executable(test_order_book tests/test_order_book.cpp)
target_link_libraries(test_order_book mm_core)
add_test(NAME order_book COMMAND test_order_book)

add_executable(test_market_maker tests/test_market_maker.cpp)
target_link_libraries(test_market_maker mm_core)
add_test(NAME market_maker COMMAND test_market_maker)

add_executable(test_agent_accounting tests/test_agent_accounting.cpp)
target_link_libraries(test_agent_accounting mm_core)
add_test(NAME agent_accounting COMMAND test_agent_accounting)

add_executable(gen_orders src/order_generator.cpp)
target_link_libraries(gen_orders mm_core)

add_executable(run_sim src/run_sim.cpp)
target_link_libraries(run_sim mm_core)