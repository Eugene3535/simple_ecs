cmake_minimum_required(VERSION 3.25.1)

set(ECS_LIB_TARGET ecslib)

set(_ecs_src_files
    src/ComponentContainer.cpp
)

add_library(${ECS_LIB_TARGET} STATIC ${_ecs_src_files})

target_compile_definitions(${ECS_LIB_TARGET} PRIVATE $<$<CONFIG:Debug>:DEBUG>)

target_include_directories(${ECS_LIB_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(${ECS_LIB_TARGET} PUBLIC cxx_std_17)
