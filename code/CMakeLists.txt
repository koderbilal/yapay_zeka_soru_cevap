cmake_minimum_required(VERSION 3.15)

project(VoxelEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STB_IMAGE_INCLUDE_DIR stb_image.h PATH_SUFFIXES stb)

file(GLOB_RECURSE VOXEL_ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(VoxelEngine ${VOXEL_ENGINE_SOURCES})
target_include_directories(VoxelEngine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(TARGET SDL2::SDL2)
    target_link_libraries(VoxelEngine PRIVATE SDL2::SDL2)
else()
    target_include_directories(VoxelEngine PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(VoxelEngine PRIVATE ${SDL2_LIBRARIES})
endif()

if(TARGET glm::glm)
    target_link_libraries(VoxelEngine PRIVATE glm::glm)
else()
    target_include_directories(VoxelEngine PRIVATE ${GLM_INCLUDE_DIRS})
endif()

target_link_libraries(VoxelEngine PRIVATE OpenGL::GL)

if(STB_IMAGE_INCLUDE_DIR)
    target_include_directories(VoxelEngine PRIVATE "${STB_IMAGE_INCLUDE_DIR}")
    target_compile_definitions(VoxelEngine PRIVATE VOXELENGINE_HAS_STB_IMAGE=1)
else()
    message(WARNING "stb_image.h was not found; a generated placeholder texture will be used.")
endif()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
