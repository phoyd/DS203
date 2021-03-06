cmake_minimum_required(VERSION 3.5)
project(dso)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmakelib")
include(extras)
max_warning()
dumpvars()

set (CMAKE_CXX_STANDARD 11)
add_executable(dso_sdl2 "")
find_package(SDL2 REQUIRED)
file(GLOB_RECURSE dso_sdl2_src 
	Source/User/_Modules.cpp 
	Source/Core/*.cpp
	Source/Framework/*.cpp 
	Source/Gui/*.cpp 
	Source/HwLayer/Common/*.cpp 
	Source/HwLayer/SDL2/*.cpp 
	Source/Library/*.cpp 
	Source/Main/*.cpp
	)
file(GLOB_RECURSE dso_sdl2_hdr 
	Source/User/*.h 
	Source/Core/*.h
	Source/Framework/*.h
	Source/Gui/*.h 
	Source/HwLayer/Types.h
        Source/HwLayer/Bios.h
        Source/HwLayer/Common/*.h
	Source/HwLayer/SDL2/*.h 
	Source/Library/*.h 
	Source/Main/*.h
	)
target_include_directories(dso_sdl2 PUBLIC ${SDL2_INCLUDE_DIRS} ./)
target_link_libraries(dso_sdl2 ${SDL2_LIBRARIES})
target_sources(dso_sdl2 PUBLIC ${dso_sdl2_src})
target_sources(dso_sdl2 PUBLIC ${dso_sdl2_hdr})
set_target_properties(dso_sdl2 PROPERTIES COMPILE_FLAGS -D_SDL2)

