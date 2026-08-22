
if(ON)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 26)

cmake_minimum_required(VERSION 4.0)

set(PROJECT_NAME "prog")
project(${PROJECT_NAME} CXX)

set(LIBROMFS_PROJECT_NAME ${PROJECT_NAME})
set(LIBROMFS_RESOURCE_LOCATION "${CMAKE_SOURCE_DIR}/romfs")
add_subdirectory(libromfs)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PRIVATE include
	PRIVATE include/imgui
	PRIVATE include/core
	PRIVATE include/core/gl
	PRIVATE include/core/sdl
	PRIVATE include/core/std
	PRIVATE include/glad
	PRIVATE include/stb
	PRIVATE include/exprtk
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

	#PRIVATE src/imgui/imgui_md.cpp

	PRIVATE src/imgui/implot3d.cpp
	PRIVATE src/imgui/implot3d_demo.cpp
	PRIVATE src/imgui/implot3d_items.cpp
	PRIVATE src/imgui/implot3d_meshes.cpp

	PRIVATE src/imgui/imgui_toggle.cpp
	PRIVATE src/imgui/imgui_toggle_palette.cpp
	PRIVATE src/imgui/imgui_toggle_presets.cpp
	PRIVATE src/imgui/imgui_toggle_renderer.cpp

	PRIVATE src/imgui/imnodes.cpp

	PRIVATE src/glad/gl.cpp

	#PRIVATE src/stb/stb_vorbis.c
	PRIVATE src/stb/impl/stb_image_write.cpp
)


target_sources(${PROJECT_NAME}
	PRIVATE src/core/main.cpp
	PRIVATE src/core/loop.cpp
)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(md4c REQUIRED)

target_link_libraries(${PROJECT_NAME} ${LIBROMFS_LIBRARY})
target_link_libraries(${PROJECT_NAME} SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL)
#target_link_libraries(${PROJECT_NAME} md4c::md4c)
