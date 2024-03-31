# Specify minimum required version
cmake_minimum_required(VERSION 3.22)

# Add Project
project(Minesweeper LANGUAGES CXX)

# FetchContent automatically downloads SFML from GitHub and builds it alongside your own code. 
# Beyond the convenience of not having to install SFML yourself, this ensures ABI compatability and
# simplifies things like specifying static versus shared libraries.
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.x) # This project uses SFML version 2.5
FetchContent_MakeAvailable(SFML)

# Create executable
add_executable(Minesweeper src/minesweeper.cpp src/board.cpp src/tile.cpp src/textures.cpp)

# Add include files
target_include_directories(Minesweeper PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Install SFML library
target_link_libraries(Minesweeper PRIVATE sfml-graphics)
target_compile_features(Minesweeper PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET Minesweeper POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Minesweeper> $<TARGET_FILE_DIR:Minesweeper> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS Minesweeper)