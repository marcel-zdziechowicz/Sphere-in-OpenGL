cmake_minimum_required(VERSION 3.26.3)
project(Sphere-in-OpenGL)

file(TestSources 
    source/app1b.c
    source/app1bproc.c 
    source/lights.c 
    source/timer.c 
    source/utilities.c 
    source/app1b.h 
    source/lights.h 
    source/openglheader.h 
    source/utilities.h 
)
add_executable(${PROJECT_NAME} ${TestSources})
#target_precompile_headers(${PROJECT_NAME} PRIVATE source/*.h)

# glfw
add_subdirectory(third_party/glfw EXCLUDE_FROM_ALL)

# GLAD
add_subdirectory(third_party/glad EXCLUDE_FROM_ALL)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad)