cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(ecs LANGUAGES CXX)

file(GLOB_RECURSE ECS_SRC_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${ECS_SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
