cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR) 
project(sudoku VERSION 0.1.0 LANGUAGES CXX C)

set(BIN_NAME "sudoku") 
add_executable(${BIN_NAME} "")

file(GLOB_RECURSE c_source "src/*.c") 
target_sources(${BIN_NAME} PRIVATE ${c_source})

target_include_directories(${BIN_NAME} PUBLIC 
	$<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE: ${CMAKE_CURRENT_BINARY_DIR}/include>)

target_include_directories(${BIN_NAME} PRIVATE 
	$<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_compile_features(${BIN_NAME} PRIVATE cxx_std_17)
set_target_properties(sudoku PROPERTIES
	C_VISIBILITY_PRESET hidden 
	CXX_VISIBILITY_PRESET hidden 
	VISIBILITY_INLINES_HIDDEN ON)

