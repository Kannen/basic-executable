cmake_minimum_required(VERSION 3.11)

project(basic-executable VERSION 0.1 
             DESCRIPTION "minimum to make an executable without stdandard libraries"
             LANGUAGES CXX ASM)

add_library(execution-start STATIC _start.S)

  target_compile_options(execution-start 
	  PUBLIC -fpie -fno-stack-protector)
  target_link_options(execution-start INTERFACE -static-pie -nostartfiles)
  target_include_directories(execution-start 
	  INTERFACE $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:"">)

  install(TARGETS execution-start EXPORT basic-executable-target)
  install(FILES execution-start.hpp
  	TYPE INCLUDE) 

add_library(gcc-expected STATIC stdlib_replacement.cpp)
  target_compile_options(gcc-expected INTERFACE
  	-fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-stack-protector
	PRIVATE -std=c++17)
  target_link_options(gcc-expected INTERFACE -nodefaultlibs)
  install(TARGETS gcc-expected EXPORT basic-executable-target)

#Installation
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/basic-executable-config-version.cmake"
	COMPATIBILITY ExactVersion 
	)
install(FILES basic-executable-config.cmake
	"${CMAKE_CURRENT_BINARY_DIR}/basic-executable-config-version.cmake"
	DESTINATION share/cmake/basic-executable) 

install(EXPORT basic-executable-target DESTINATION share/cmake/basic-executable)
