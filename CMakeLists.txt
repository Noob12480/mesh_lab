cmake_minimum_required(VERSION 3.16)

project(mesh_weaver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/utf-8)
endif()

file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(mesh_weaver
    ${SRC_FILES}
)

target_include_directories(mesh_weaver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/eigen
)

target_compile_definitions(mesh_weaver PRIVATE UNICODE _UNICODE)

target_link_libraries(mesh_weaver PRIVATE
    user32
    gdi32
)