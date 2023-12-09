cmake_minimum_required (VERSION 3.20)
project(
  "Learning BGL" 
  VERSION 1.0 
  DESCRIPTION "Repository with examples for understanding how BGL works"
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)
add_executable(
  learning_bgl
  "src/kevin-bacon.cpp"
)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message( STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS Graph)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

target_include_directories(learning_bgl
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src"
    )

target_link_libraries(
  learning_bgl
  PRIVATE
  ${Boost_LIBRARIES}
)

add_custom_command(
TARGET learning_bgl POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy
${CMAKE_SOURCE_DIR}/src/kevin-bacon.dat
${CMAKE_CURRENT_BINARY_DIR}/kevin-bacon.dat)


