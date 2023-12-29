cmake_minimum_required(VERSION 3.26.3)
project(Sphere-in-OpenGL)

file(GLOB_RECURSE TestSources source/*.c source/*.h)
add_executable(${PROJECT_NAME} ${TestSources})
#target_precompile_headers(${PROJECT_NAME} PRIVATE source/*.h)

# glfw
add_subdirectory(third_party/glfw EXCLUDE_FROM_ALL)

# GLAD
add_subdirectory(third_party/glad EXCLUDE_FROM_ALL)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad)