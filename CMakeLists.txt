cmake_minimum_required(VERSION 3.16)

# set(HUNTER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/hunter" )
# set(HUNTER_ROOT "${CMAKE_CURRENT_BINARY_DIR}/hunter" )

include("cmake/HunterGate.cmake")

set(HUNTER_NO_TOOLCHAIN_ID_RECALCULATION ON)
set(HUNTER_CONFIGURATION_TYPES "Release" CACHE STRING "Hunter build Type" FORCE)
set(HUNTER_STATUS_DEBUG OFF)
set(HUNTER_STATUS_PRINT ON)
set(HUNTER_PACKAGE_LOG_BUILD ON)
set(HUNTER_PACKAGE_LOG_INSTALL ON)
set(HUNTER_BUILD_SHARED_LIBS OFF)
set(HUNTER_KEEP_PACKAGE_SOURCE ON)
set(CMAKE_FIND_DEBUG_MODE ON)

set(CMAKE_BUILD_TYPE ${HUNTER_CONFIGURATION_TYPES})
set(BUILD_SHARED_LIBS ${HUNTER_BUILD_SHARED_LIBS})

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#set(CMAKE_SKIP_INSTALL_RULES ON)
#set(CMAKE_SKIP_PACKAGE_ALL_DEPENDENCY ON)
#set(CMAKE_SUPPRESS_REGENERATION ON)

HunterGate(
    URL  "https://github.com/glencjones/hunter/archive/refs/tags/v0.23.314-alloy-1.tar.gz"
    SHA1 "4549223ba9d965c1cd332b2e99d566cfe010e06a"
)

execute_process(
  COMMAND git log -1 --pretty=format:%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE VERSION
)

add_definitions(-DGIT_REVISION="${VERSION}")

project(Alloy)

###############################################################
##  Fetch Boot
##
hunter_add_package(Boost COMPONENTS program_options filesystem)
find_package(Boost CONFIG REQUIRED program_options filesystem)
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

set(CMAKE_FIND_DEBUG_MODE FALSE)

if(MSVC)
elseif(APPLE)
else()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -maes -fno-inline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes -fno-inline")
endif()

include_directories(include src "${CMAKE_CURRENT_BINARY_DIR}/version")
link_libraries(Boost::boost)

if(MSVC)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Windows)
elseif(APPLE)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/OSX)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Posix)
else()
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Linux)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Posix)
endif()

add_definitions("-D_DEFAULT_SOURCE -D_GNU_SOURCE")

set(COMMIT_ID_IN_VERSION ON CACHE BOOL "Include commit ID in version")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/version")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in" "${CMAKE_CURRENT_BINARY_DIR}/version/version.h")
add_custom_target(version ALL)

add_subdirectory(src)

## Tests are not working will fix in next release, seems to be previous code issues again.
## TODO Place correct option handling around tests
#add_subdirectory(tests)
