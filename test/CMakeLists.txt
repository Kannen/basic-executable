cmake_minimum_required(VERSION 3.11)

project(test-basic-executable VERSION 0.1 
             DESCRIPTION "minimum to make an executable without stdandard libraries"
             LANGUAGES CXX)

find_package(fcpp)
find_package(kernel-api)

add_compile_options(-ftrack-macro-expansion=0)

get_target_property(INCLUDE_DIRS elf INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(INCLUDE_DIR_ kernel-api INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${INCLUDE_DIR_}) 
get_target_property(INCLUDE_DIR_ elf INTERFACE_INCLUDE_DIRECTORIES)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${INCLUDE_DIR_}) 
get_target_property(INCLUDE_DIR_ fcpp INTERFACE_INCLUDE_DIRECTORIES)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${INCLUDE_DIR_}) 
list(JOIN INCLUDE_DIRS , INCLUDE_DIRS)

configure_file(vimrc.inc ${PROJECT_BINARY_DIR}/vimrc @ONLY)

add_library(dyn-assert OBJECT dynamic-assertion.cpp)
  target_link_libraries(dyn-assert fcpp kernel-api)

add_executable(assertion assertion.cpp common_reference.cpp $<TARGET_OBJECTS:dyn-assert>)
  target_link_libraries(assertion PRIVATE elf fcpp kernel-api)

add_executable(array array.cpp $<TARGET_OBJECTS:dyn-assert>)
  target_link_libraries(array PRIVATE elf fcpp kernel-api)

add_executable(tuple tuple.cpp $<TARGET_OBJECTS:dyn-assert>)
  target_link_libraries(tuple PRIVATE elf fcpp kernel-api)

add_executable(visitor visitor.cpp $<TARGET_OBJECTS:dyn-assert>)
  target_link_libraries(visitor PRIVATE elf fcpp kernel-api)

add_executable(variant variant.cpp $<TARGET_OBJECTS:dyn-assert>)
  target_link_libraries(variant PRIVATE elf fcpp kernel-api)

