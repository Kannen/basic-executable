cmake_minimum_required(VERSION 3.11)

project(basic-execution VERSION 0.1 
             DESCRIPTION "minimum to make an elf execution"
             LANGUAGES CXX ASM)

add_library(execution-start STATIC _start.S)

  target_compile_options(execution-start INTERFACE
  	-static)
  target_link_options(execution-start INTERFACE -static -nostartfiles)
  target_include_directories(execution-start 
	  INTERFACE $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:"">)

  install(TARGETS execution-start EXPORT basic-execution-target)
  install(FILES execution-start.hpp
  	TYPE INCLUDE) 

add_library(gcc-expected STATIC stdlib_replacement.cpp)
  target_compile_options(gcc-expected INTERFACE
  	-fno-exceptions -fno-rtti -fno-threadsafe-statics
	PRIVATE -std=c++17)
  target_link_options(execution-start INTERFACE -nodefaultlibs)
  install(TARGETS gcc-expected EXPORT basic-execution-target)

#Installation
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/basic-execution-config-version.cmake"
	COMPATIBILITY ExactVersion 
	)
install(FILES basic-execution-config.cmake "${CMAKE_CURRENT_BINARY_DIR}/basic-execution-config-version.cmake"
	DESTINATION share/cmake/basic-execution) 

install(EXPORT basic-execution-target DESTINATION share/cmake/basic-execution)
