cmake_minimum_required(VERSION 3.15)

project(VoxelEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm CONFIG QUIET)

file(GLOB_RECURSE VOXEL_ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(VoxelEngine ${VOXEL_ENGINE_SOURCES})
target_include_directories(VoxelEngine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_compile_definitions(VoxelEngine PRIVATE SDL_MAIN_HANDLED)

if(TARGET SDL2::SDL2)
    target_link_libraries(VoxelEngine PRIVATE SDL2::SDL2)
else()
    target_include_directories(VoxelEngine PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(VoxelEngine PRIVATE ${SDL2_LIBRARIES})
endif()

if(TARGET glm::glm)
    target_link_libraries(VoxelEngine PRIVATE glm::glm)
else()
    find_path(GLM_INCLUDE_DIR glm/glm.hpp REQUIRED)
    target_include_directories(VoxelEngine PRIVATE ${GLM_INCLUDE_DIR})
endif()

target_link_libraries(VoxelEngine PRIVATE OpenGL::GL)

add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/assets"
)
