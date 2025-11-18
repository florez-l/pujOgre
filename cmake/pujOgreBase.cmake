## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

# Decide whether links are shared or static
if(EMSCRIPTEN)
  set(pujOgre_BUILD_TYPE STATIC)
  option(pujOgre_BUILD_WASM_WITH_ASSERTIONS "Build with WASM assertions?" OFF)
else()
  if(WIN32 OR APPLE OR MINGW OR MSYS OR CYGWIN)
    set(pujOgre_BUILD_TYPE STATIC)
  else()
    set(pujOgre_BUILD_TYPE SHARED)
  endif()
endif()

# Set a default build type if none was specified
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(
    CMAKE_BUILD_TYPE "${default_build_type}" CACHE
    STRING "Choose the type of build." FORCE
    )
  # Set the possible values of build type for cmake-gui
  set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
    )
endif()

# Output dirs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# C++ version
set(CMAKE_CXX_VERSION 23)

# CMake directives
include(GenerateExportHeader)

# Find packages
find_package(OGRE REQUIRED)

## eof - pujOgreBase.cmake
