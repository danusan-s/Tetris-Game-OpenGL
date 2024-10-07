cmake_minimum_required(VERSION 3.10)
project(TetrisGame)

set(CMAKE_CXX_STANDARD 17)

# Find dependencies
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

# Collect all source files in src/ folder
file(GLOB SOURCES "src/*.cpp" "src/*.c")

# Include header files from the include/ folder
include_directories(include)

# Add executable
add_executable(TetrisGame ${SOURCES})

# Link libraries
target_link_libraries(TetrisGame
    glfw
    OpenGL::GL
    ${FREETYPE_LIBRARIES}
)

# Include directories for FreeType and the include/ folder
target_include_directories(TetrisGame
    PUBLIC
        ${FREETYPE_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include  # Include the headers in the include/ directory
)

# Platform-specific settings
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows-specific options
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Linux-specific options
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # macOS-specific options
endif()

