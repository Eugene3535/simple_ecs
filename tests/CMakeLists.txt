cmake_minimum_required(VERSION 3.25.1)

set(ECS_TESTS_TARGET ecs-tests)

set(_tests_src_files
    main.cpp
)

add_executable(${ECS_TESTS_TARGET} ${_tests_src_files})

target_compile_definitions(${ECS_TESTS_TARGET} PRIVATE $<$<CONFIG:Debug>:DEBUG>)

target_compile_features(${ECS_TESTS_TARGET} PUBLIC cxx_std_17)

target_link_libraries(${ECS_TESTS_TARGET} PRIVATE ecslib)