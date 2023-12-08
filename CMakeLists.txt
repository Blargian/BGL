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
  "src/main.cpp"
)

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


