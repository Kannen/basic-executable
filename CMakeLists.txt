cmake_minimum_required(VERSION 3.11)

project(basic-executable VERSION 0.1 
             DESCRIPTION "minimum to make an executable without stdandard libraries"
             LANGUAGES CXX ASM)

execute_process(COMMAND 
  bash -c "g++ --version -v |& grep includedir | sed 's/.*includedir=\\([^ ]*\\).*/\\1/'"
  OUTPUT_VARIABLE GCC_INCLUDE_DIR)
execute_process(COMMAND 
  bash -c "g++ --version -v |& grep includedir | sed 's/.*gxx-include-dir=\\([^ ]*\\).*/\\1/'"
  OUTPUT_VARIABLE GXX_INCLUDE_DIR)

string(STRIP ${GXX_INCLUDE_DIR} GXX_INCLUDE_DIR)
string(STRIP ${GCC_INCLUDE_DIR} GCC_INCLUDE_DIR)
message(${GCC_INCLUDE_DIR})
message(${GXX_INCLUDE_DIR})

add_library(start STATIC _start.S)

  target_compile_options(start 
	  PUBLIC -fno-stack-protector -fno-exceptions -fno-rtti
	  -fno-asynchronous-unwind-tables
	  -fno-unwind-tables -fshort-enums -static)
  target_link_options(start INTERFACE -nostartfiles
	  PUBLIC -static -Wl,--no-eh-frame-hdr,--no-ld-generated-unwind-info)
  target_include_directories(start 
	  INTERFACE $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

  install(TARGETS start EXPORT basic-executable-target)
  install(FILES include/start.hpp TYPE INCLUDE) 

add_library(gcc-expected STATIC stdlib_replacement.cpp)
  target_include_directories(gcc-expected 

	  PUBLIC $<INSTALL_INTERFACE:include/to-sys/g++>
	         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/to-sys/g++>

	  PUBLIC $<INSTALL_INTERFACE:include/to-sys/g++x86>
	         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/to-sys/g++x86>

	  PUBLIC $<INSTALL_INTERFACE:include/to-sys/gcc>
	         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/to-sys/gcc>

	  PUBLIC $<INSTALL_INTERFACE:include>
	         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>

	  PUBLIC $<INSTALL_INTERFACE:include/to-sys>
	         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/to-sys>

	  )
  target_compile_options(gcc-expected PUBLIC
    	-fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-stack-protector
  	-ffreestanding -fno-asynchronous-unwind-tables
  	-fno-unwind-tables -fshort-enums
  	-nostdinc
  	-nostdinc++
  	-fno-operator-names
  	-ffreestanding
  	PRIVATE -std=c++17)
  target_link_options(gcc-expected INTERFACE -nodefaultlibs)
  install(TARGETS gcc-expected EXPORT basic-executable-target)
  install(DIRECTORY include/to-sys TYPE INCLUDE)
  install(FILES include/c++support.hpp TYPE INCLUDE)
  install(FILES include/assertion.hpp TYPE INCLUDE)
  install(FILES include/array.hpp TYPE INCLUDE)
  install(FILES include/tuple.hpp TYPE INCLUDE)
  install(FILES include/visitor.hpp TYPE INCLUDE)


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

