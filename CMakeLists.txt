
cmake_minimum_required(VERSION 2.8)

project(spectrum_planner)

include_directories(${CMAKE_SOURCE_DIR}/src)

# set warning level
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")


# Load the cmake directory to submodule path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)


# go get glfw3 (instructions from glfw docs)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

# go get OpenGL
find_package(OpenGL REQUIRED)

# go get glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# go get FreeImage
find_package(FreeImage REQUIRED)
include_directories(${FREEIMAGE_INCLUDE_PATH})

# point to the resource directory for textures, etc.
add_definitions(-DRESOURCE_DIR="${CMAKE_SOURCE_DIR}/res")

# Glob up the files
file(GLOB APP_FILES ${CMAKE_SOURCE_DIR}/src/*.*pp)
file(GLOB MATH_FILES ${CMAKE_SOURCE_DIR}/src/math/*.*pp)
file(GLOB SHADER_FILES ${CMAKE_SOURCE_DIR}/src/shaders/*.*pp)
file(GLOB UTIL_FILES ${CMAKE_SOURCE_DIR}/src/util/*.*pp)

# make the executable
add_executable(spectrum_planner ${APP_FILES} 
	${MATH_FILES} 
	${SHADER_FILES} 
	${UTIL_FILES})

# link against glfw3, opengl
target_link_libraries(spectrum_planner ${GLFW_STATIC_LIBRARIES})
target_link_libraries(spectrum_planner ${OPENGL_LIBRARIES})
target_link_libraries(spectrum_planner ${GLEW_LIBRARIES})
target_link_libraries(spectrum_planner ${FREEIMAGE_LIBRARIES})
