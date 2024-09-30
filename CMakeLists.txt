cmake_minimum_required(VERSION 3.20)
project(ITCHLimitOrderbook)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(GNUInstallDirs)

find_package(Boost REQUIRED COMPONENTS system iostreams)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/orderbook-main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE /include)

include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)

myproject_set_project_warnings(${PROJECT_NAME} TRUE "X" "" "" "X")

add_compile_options(-pipe -fPIC)
myproject_enable_sanitizers(${PROJECT_NAME} TRUE TRUE TRUE FALSE TRUE)

target_link_libraries(
  ${PROJECT_NAME}
  LINK_PUBLIC
  ${Boost_LIBRARIES})

