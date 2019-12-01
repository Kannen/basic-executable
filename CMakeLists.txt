cmake_minimum_required(VERSION 3.11)

project(basic-executable VERSION 0.1 
             DESCRIPTION "minimum to make an executable without stdandard libraries"
             LANGUAGES CXX ASM)

add_library(start STATIC _start.S)

  target_compile_options(start 
	  PUBLIC -fno-stack-protector -fno-exceptions -fno-rtti
	  -fno-asynchronous-unwind-tables
	  -fno-unwind-tables -fshort-enums -static)
  target_link_options(start INTERFACE -nostartfiles
	  PUBLIC -static -Wl,--no-eh-frame-hdr,--no-ld-generated-unwind-info)
  target_include_directories(start 
	  INTERFACE $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:"">)

  install(TARGETS start EXPORT basic-executable-target)
  install(FILES start.hpp
  	TYPE INCLUDE) 

add_library(gcc-expected STATIC stdlib_replacement.cpp)
target_compile_options(gcc-expected PUBLIC
  	-fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-stack-protector
	-ffreestanding -fno-asynchronous-unwind-tables
	-fno-unwind-tables -fshort-enums
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

