## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

cmake_minimum_required(VERSION 3.20)
project(PUJ_Ogre)

# -- Some configurations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# -- Check build path
if(${PROJECT_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
  message(FATAL_ERROR "Build is not allowed in the same base path.")
endif()

# -- Set a default build type if none was specified
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    STATUS
    "Setting build type to '${default_build_type}' as none was specified."
    )
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

# -- Library creation helpers
include(GenerateExportHeader)

# -- Packages
find_package(OGRE REQUIRED)
find_package(Bullet REQUIRED)
find_package(VTK REQUIRED)

# -- Ogre Plugins
set(PUJ_Ogre_Plugins "" CACHE STRING "")
foreach(_plg ${OGRE_PLUGINS})
  string(
    CONCAT PUJ_Ogre_Plugins
    "${PUJ_Ogre_Plugins}" "${OGRE_PLUGIN_DIR}/${_plg}|"
    )
endforeach()
file(GLOB _plgs ${PROJECT_SOURCE_DIR}/plugins/*)
foreach(_plg ${_plgs})
  if(IS_DIRECTORY ${_plg})
    string(
      REPLACE
      "${PROJECT_SOURCE_DIR}/plugins/" "Plugin_PUJ_Ogre_"
      _plg ${_plg}
      )
    string(
      CONCAT PUJ_Ogre_Plugins
      "${PUJ_Ogre_Plugins}" "${_plg}|"
      )
  endif()
endforeach()

# -- Targets
subdirs(apps bash plugins lib)

## eof - CMakeLists.txt
