## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

# Source code
set(_name Plugin_pujOgreDotScenePPCodec)
set(_hdr OgreDotPPSceneLoader.h ParametricSurfaceBuilder.h)
set(_src OgreDotPPSceneLoader.cxx ParametricSurfaceBuilder.cxx)

# Build library
include_directories(
  ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}
   ${PROJECT_SOURCE_DIR}/plugins/DotScenePP
  ${PROJECT_BINARY_DIR}/lib ${PROJECT_SOURCE_DIR}/lib
  )
add_library(${_name} SHARED ${_src})
generate_export_header(
  ${_name}
  EXPORT_MACRO_NAME ${_name}_EXPORT
  EXPORT_FILE_NAME Export.h
  )
target_link_libraries(${_name} PRIVATE pugixml pujOgre OgreTerrain)
set_target_properties(
  ${_name} PROPERTIES
  PREFIX ""
  CXX_VISIBILITY_PRESET hidden
  )
vtk_module_autoinit(TARGETS ${_name} MODULES ${VTK_LIBRARIES})

# file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
# list(APPEND HEADER_FILES ${PROJECT_BINARY_DIR}/include/OgreDotScenePluginExports.h)

# find_package(VTK REQUIRED COMPONENTS
#     CommonCore CommonDataModel CommonComputationalGeometry CommonMisc FiltersCore FiltersSources)

# add_library(Plugin_DotScene ${OGRE_LIB_TYPE} ${HEADER_FILES}
#     src/DotSceneLoader.cpp
#     src/ParametricSurfaceBuilder.cpp
#     src/VtkParametricExpression.cpp
#     src/VtkToOgreMesh.cpp)
# target_include_directories(Plugin_DotScene PUBLIC
#     "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
#     $<INSTALL_INTERFACE:include/OGRE/Plugins/DotScene>)
# target_link_libraries(Plugin_DotScene PUBLIC OgreMain PRIVATE pugixml ${VTK_LIBRARIES})
# target_compile_definitions(Plugin_DotScene PRIVATE VTKOGRE_BUILD)
# vtk_module_autoinit(TARGETS Plugin_DotScene MODULES ${VTK_LIBRARIES})
# if(OGRE_BUILD_COMPONENT_TERRAIN)
#     target_link_libraries(Plugin_DotScene PUBLIC OgreTerrain)
# endif()

# ogre_config_framework(Plugin_DotScene)
# ogre_config_plugin(Plugin_DotScene)
# generate_export_header(Plugin_DotScene
#     EXPORT_MACRO_NAME _OgreDotScenePluginExport
#     EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/OgreDotScenePluginExports.h)
# install(FILES ${HEADER_FILES} DESTINATION include/OGRE/Plugins/DotScene)

## eof - CMakeLists.txt
