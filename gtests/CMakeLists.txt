# filepath: /home/andrew-gilbert/reinforcement-learning-projects/BackGammon/gtests/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(BackGammonTests)

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add the test executable
add_executable(BoardTest BoardTest.cpp ../logic/Board.c++)

# Link against Google Test and pthread
target_link_libraries(BoardTest ${GTEST_LIBRARIES} pthread)