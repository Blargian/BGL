cmake_minimum_required (VERSION 3.20)
project(
  "Learning BGL" 
  VERSION 1.0 
  DESCRIPTION "Repository with examples for understanding how BGL works"
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message( STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# ----- begin BOOST related ----- # 

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS Graph)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

include_directories(
"${CMAKE_CURRENT_LIST_DIR}/src"
)

# link boost to all targets
link_libraries(
  ${Boost_LIBRARIES}
)

# ----- begin target definitions ----- # 

add_executable(
  kevin-bacon
  "src/kevin-bacon.cpp"
)

add_executable(
  kruskal
  "src/kruskal.cpp"
)

add_executable(
  tiny-ewg
  "src/tiny-ewg.cpp"
)
# ----- end target definitions ----- # 

# ----- begin copy across needed files ----- # 

add_custom_command(
TARGET kevin-bacon POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy
${CMAKE_SOURCE_DIR}/src/kevin-bacon.dat
${CMAKE_CURRENT_BINARY_DIR}/kevin-bacon.dat
)

add_custom_command(
TARGET tiny-ewg POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy
${CMAKE_SOURCE_DIR}/src/tiny-ewg.txt
${CMAKE_CURRENT_BINARY_DIR}/tiny-ewg.txt
)

# ----- end copy across needed files ----- #
