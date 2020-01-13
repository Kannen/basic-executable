cmake_minimum_required(VERSION 3.11)

project(fcpp VERSION 0.1 
             DESCRIPTION "c++ in a freestanding environment"
             LANGUAGES CXX ASM)

execute_process(COMMAND 
  bash -c "g++ --version -v |& grep Target | sed 's/Target: \\([^ ]*\\).*/\\1/'"
  OUTPUT_VARIABLE GCC_HOST)
  string(STRIP ${GCC_HOST} GCC_HOST)

execute_process(COMMAND 
  bash -c "gcc --version | head -1 | sed 's/.*) \\([^ ]\\+\\)\\..\\..*/\\1/'"
  OUTPUT_VARIABLE GCC_VERSION_MAJOR)
  string(STRIP ${GCC_VERSION_MAJOR} GCC_VERSION_MAJOR)

execute_process(COMMAND 
  bash -c "g++ --version -v |& grep includedir | sed 's/.*includedir=\\([^ ]*\\).*/\\1/'"
  OUTPUT_VARIABLE GCC_INCLUDE_DIR)


if (GCC_INCLUDE_DIR)
  string(STRIP ${GCC_INCLUDE_DIR} GCC_INCLUDE_DIR)
else()
  execute_process(COMMAND 
    bash -c "g++ --version -v |& grep prefix | sed 's/.*prefix=\\([^ ]*\\).*/\\1/'"
    OUTPUT_VARIABLE GCC_INCLUDE_DIR)
  string(STRIP ${GCC_INCLUDE_DIR} GCC_INCLUDE_DIR)
  set(GCC_INCLUDE_DIR ${GCC_INCLUDE_DIR}/lib/gcc/${GCC_HOST}/${GCC_VERSION_MAJOR}/include)
endif()

execute_process(COMMAND 
  bash -c "g++ --version -v |& grep includedir | sed 's/.*gxx-include-dir=\\([^ ]*\\).*/\\1/'"
  OUTPUT_VARIABLE GXX_INCLUDE_DIR)

if (GXX_INCLUDE_DIR)
  string(STRIP ${GXX_INCLUDE_DIR} GXX_INCLUDE_DIR)
else()
  execute_process(COMMAND 
    bash -c "g++ --version -v |& grep prefix | sed 's/.*prefix=\\([^ ]*\\).*/\\1/'"
    OUTPUT_VARIABLE GXX_INCLUDE_DIR)
  string(STRIP ${GXX_INCLUDE_DIR} GXX_INCLUDE_DIR)
  set(GXX_INCLUDE_DIR ${GXX_INCLUDE_DIR}/include/c++/${GCC_VERSION_MAJOR})
endif()

message(${GCC_INCLUDE_DIR})
message(${GXX_INCLUDE_DIR})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys)
file(CREATE_LINK ${GCC_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys/gcc SYMBOLIC)
file(CREATE_LINK ${GXX_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys/g++ SYMBOLIC)
file(CREATE_LINK ${GXX_INCLUDE_DIR}/${GCC_HOST} ${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys/g++x86 SYMBOLIC)

set(VIMRC_INCLUDE_DIRS  ${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys/gcc
	${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys/g++
	${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys/g++x86
	${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys
	)

configure_file(vimrc.inc ${PROJECT_BINARY_DIR}/vimrc @ONLY)

add_library(fcpp STATIC cstring.cpp gcc-required.cpp)
  target_include_directories(fcpp 

	  PUBLIC $<INSTALL_INTERFACE:include>
	         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>

	  PUBLIC $<INSTALL_INTERFACE:include/fcpp/to-sys/g++>
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys/g++>

	  PUBLIC $<INSTALL_INTERFACE:include/fcpp/to-sys/g++x86>
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys/g++x86>

	  PUBLIC $<INSTALL_INTERFACE:include/fcpp/to-sys/gcc>
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/fcpp/to-sys/gcc>

	  PUBLIC $<INSTALL_INTERFACE:include/fcpp/to-sys>
	         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fcpp/to-sys>


	  )
  target_compile_options(fcpp PUBLIC
    	-fno-exceptions 
	-fno-rtti 
	-fno-threadsafe-statics 
	-fno-stack-protector
  	-fno-unwind-tables 
	-fno-asynchronous-unwind-tables
	-fshort-enums 

  	-ffreestanding 
	-static
  	-nostdinc
  	-nostdinc++
  	-fno-operator-names

	-std=c++2a

	-fdiagnostics-color=always
	-Wall
	-Wextra
	-Wnon-virtual-dtor
	-Wsign-promo
        -Wstrict-null-sentinel 
        -Wimplicit-fallthrough=5
        -Wreturn-local-addr 
	#-Wsuggest-attribute=pure 
	#-Wsuggest-attribute=const 
	-Wsuggest-attribute=noreturn
        -Wsuggest-final-types 
	-Wsuggest-final-methods
        -Warray-bounds=2 
        -Wduplicated-branches 
	-Wduplicated-cond 
        -Wplacement-new=2

	-Wno-unused-parameter
        -Wno-parentheses
	)

  target_link_options(fcpp PUBLIC 
	-nostdlib
	-static 
	-Wl,--no-eh-frame-hdr,--no-ld-generated-unwind-info
	)
  install(TARGETS fcpp EXPORT fcpp-target)
  install(DIRECTORY fcpp TYPE INCLUDE REGEX ".*.sw" EXCLUDE)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fcpp TYPE INCLUDE REGEX ".*.sw" EXCLUDE)


#Installation
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/fcpp-config-version.cmake"
	COMPATIBILITY ExactVersion 
	)
install(FILES fcpp-config.cmake
	"${CMAKE_CURRENT_BINARY_DIR}/fcpp-config-version.cmake"
	DESTINATION share/cmake/fcpp) 

install(EXPORT fcpp-target DESTINATION share/cmake/fcpp)

