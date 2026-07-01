set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 26)

set(CMAKE_BUILD_TYPE Debug)

cmake_minimum_required(VERSION 4.0)

set(PROJECT_NAME "template")

project(${PROJECT_NAME} CXX)

add_executable(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
	PRIVATE include
	PRIVATE include/imgui
	PRIVATE include/core
	PRIVATE include/glad
)

target_sources(${PROJECT_NAME}
	PRIVATE src/imgui/imgui.cpp
	PRIVATE src/imgui/imgui_demo.cpp
	PRIVATE src/imgui/imgui_draw.cpp
	PRIVATE src/imgui/imgui_tables.cpp
	PRIVATE src/imgui/imgui_widgets.cpp
	PRIVATE src/imgui/imgui_impl_opengl3.cpp
	PRIVATE src/imgui/imgui_impl_sdl3.cpp

	PRIVATE src/imgui/implot.cpp
	PRIVATE src/imgui/implot_demo.cpp
	PRIVATE src/imgui/implot_items.cpp

	PRIVATE src/imgui/implot3d.cpp
	PRIVATE src/imgui/implot3d_demo.cpp
	PRIVATE src/imgui/implot3d_items.cpp
	PRIVATE src/imgui/implot3d_meshes.cpp

	PRIVATE src/imgui/imnodes.cpp

	PRIVATE src/glad/gl.cpp
)


target_sources(${PROJECT_NAME}
	PRIVATE src/core/main.cpp
	PRIVATE src/core/loop.cpp
)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL)
