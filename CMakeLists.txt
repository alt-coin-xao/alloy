cmake_minimum_required(VERSION 3.16)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

message(STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} mode")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  message(STATUS "Building DEBUG mode")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -ggdb3" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -ggdb3" CACHE STRING "" FORCE)
  SET(HUNTER_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)
  find_package(Clangd)
  IF(Clangd_FOUND)
    message(STATUS "Development (IDE) information added clangd")
    ADDCLANGDLSP()
  ENDIF()
else()
  message(STATUS "Building RELEASE mode")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
  SET(HUNTER_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
endif()

include("cmake/HunterGate.cmake")
#set(HUNTER_NO_TOOLCHAIN_ID_RECALCULATION ON)
#set(HUNTER_STATUS_DEBUG OFF)
#set(HUNTER_STATUS_PRINT ON)
#set(HUNTER_PACKAGE_LOG_BUILD ON)
#set(HUNTER_PACKAGE_LOG_INSTALL ON)
#set(HUNTER_BUILD_SHARED_LIBS ON)
#set(HUNTER_KEEP_PACKAGE_SOURCE ON)
#set(CMAKE_FIND_DEBUG_MODE ON)
#set(MINIUPNPC_USE_STATIC_LIBS ON)

HunterGate(
    URL  "https://github.com/glencjones/hunter/archive/refs/tags/v0.23.314-alloy-1.tar.gz"
    SHA1 "4549223ba9d965c1cd332b2e99d566cfe010e06a"
    )

execute_process(
  COMMAND git log -1 --pretty=format:%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND git describe --tags --dirty --match "v*"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_LABEL
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

message(STATUS "GIT_VERSION ${GIT_VERSION}")
message(STATUS "GIT_LABEL ${GIT_LABEL}")

add_definitions(-DGIT_REVISION="${GIT_VERSION}")

project(Alloy)

###############################################################
##  Fetch Boot
##
hunter_add_package(Boost COMPONENTS program_options filesystem serialization chrono)
find_package(Boost CONFIG REQUIRED program_options filesystem serialization chrono)
###
###############################################################

###############################################################
## Fetch miniupnpc
##
hunter_add_package(miniupnpc)
find_package(miniupnpc CONFIG REQUIRED)
###
###############################################################

###############################################################
## Fetch rocksdb
##
hunter_add_package(rocksdb)
find_package(RocksDB CONFIG REQUIRED) 
###
###############################################################

###############################################################
## Fetch GTest
##
##
hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)
##
###############################################################

if(MSVC)
add_definitions("/bigobj /MP /w /W0 /GS- /D_CRT_SECURE_NO_WARNINGS /wd4996 /wd4345 /D_WIN32_WINNT=0x0600 /DWIN32_LEAN_AND_MEAN /DGTEST_HAS_TR1_TUPLE=0 /D_VARIADIC_MAX=8 /D__SSE4_1__")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:10485760")
add_definitions("-DBOOST_UUID_RANDOM_GENERATOR_COMPAT")
elseif(APPLE)
else()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -maes -fno-inline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes -fno-inline -std=gnu++14")
add_definitions("-D_DEFAULT_SOURCE -D_GNU_SOURCE")
endif()

include_directories(include src "${CMAKE_CURRENT_BINARY_DIR}/version")
link_libraries(Boost::boost)

if(MSVC)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Windows)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/msc)
elseif(APPLE)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/OSX)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Posix)
else()
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Linux)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Posix)
endif()

set(COMMIT_ID_IN_VERSION ON CACHE BOOL "Include commit ID in version")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/version")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in" "${CMAKE_CURRENT_BINARY_DIR}/version/version.h")
add_custom_target(version ALL)

add_subdirectory(src)

if(UNIX)
  option(BUILD_TESTS "Build Tests" OFF)
  option(TESTS_WITH_ALL "Add Tests to all" OFF)
  if(BUILD_TESTS)
    enable_testing()
    add_subdirectory( tests )
  endif(BUILD_TESTS)
endif(UNIX)
