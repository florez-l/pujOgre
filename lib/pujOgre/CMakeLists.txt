## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

# Source code
set(_name pujOgre)
set(
  _hdr
  BaseApplication.h
  FPSCamera.h
  vtkParametricExpression.h
  vtkPolyData2OgreMesh.h
  )
set(
  _src
  BaseApplication.cxx
  FPSCamera.cxx
  vtkParametricExpression.cxx
  vtkPolyData2OgreMesh.cxx
  )

# Build library
include_directories(${PROJECT_BINARY_DIR}/lib ${PROJECT_SOURCE_DIR}/lib)
add_library(${_name} SHARED ${_src})
generate_export_header(
  ${_name}
  EXPORT_MACRO_NAME ${_name}_EXPORT
  EXPORT_FILE_NAME Export.h
  )
target_link_libraries(${_name} PUBLIC OgreMain OgreBites ${VTK_LIBRARIES})
list(JOIN pujOgre_PLUGINS "|" _plugins)
target_compile_definitions(
  ${_name}
  PRIVATE
  pujOgre_PLUGINS="${_plugins}"
  pujOgre_SHARED_EXT="${CMAKE_SHARED_LIBRARY_SUFFIX}"
  )
vtk_module_autoinit(TARGETS ${_name} MODULES ${VTK_LIBRARIES})

## eof - CMakeLists.txt
