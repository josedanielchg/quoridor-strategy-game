cmake_minimum_required(VERSION 3.22)
project(QuoridorGame VERSION 0.1 LANGUAGES CXX)

# Force C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------
# AUTOMATICALLY DOWNLOAD AND BUILD SFML 3.0
# --------------------------------------------------------
include(FetchContent)

# 1. Configure SFML to build statically (Easier for Windows - No DLL copying needed!)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) # Static linking

# 2. Fetch the library from GitHub
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0
)

# 3. Make it available to the project
message(STATUS "Downloading and compiling SFML 3.0...")
FetchContent_MakeAvailable(SFML)

# --------------------------------------------------------
# PROJECT CONFIGURATION
# --------------------------------------------------------

# Define your source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include headers (all of them are in include/ folder)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Link SFML (CMake knows where they are now because it built them)
# Note: SFML 3 target names are case-sensitive namespaces
target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio
    SFML::Network
)
if(WIN32)
    target_link_libraries(QuoridorGame PRIVATE SFML::Main)
endif()

# --------------------------------------------------------
# ASSETS (Copy assets folder to build directory)
# --------------------------------------------------------
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
        COMMENT "Copying assets to output directory..."
    )
endif()
