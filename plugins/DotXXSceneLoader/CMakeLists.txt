## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

set(
  _hdr
  PUJ_Ogre/DotXXSceneLoader.h
  PUJ_Ogre/third_party/ev3/auxiliary.h
  PUJ_Ogre/third_party/ev3/common.h
  PUJ_Ogre/third_party/ev3/exceptions.h
  PUJ_Ogre/third_party/ev3/expression.h
  PUJ_Ogre/third_party/ev3/operand.h
  PUJ_Ogre/third_party/ev3/parser.h
  PUJ_Ogre/third_party/ev3/tree.h
  PUJ_Ogre/third_party/exprtk.h
  PUJ_Ogre/vtk/ParametricFunction.h
  )
set(
  _src
  PUJ_Ogre/DotXXSceneLoader.cxx
  PUJ_Ogre/DotXXSceneLoader_parametric2.cxx
  PUJ_Ogre/DotXXSceneLoader_vtk.cxx
  PUJ_Ogre/third_party/ev3/expression.cxx
  PUJ_Ogre/third_party/ev3/operand.cxx
  PUJ_Ogre/third_party/ev3/parser.cxx
  PUJ_Ogre/vtk/ParametricFunction.cxx
  )

include_directories(
  ${PROJECT_BINARY_DIR}/plugins/DotXXSceneLoader
  ${PROJECT_SOURCE_DIR}/plugins/DotXXSceneLoader
  )
add_library(Plugin_PUJ_Ogre_DotXXSceneLoader SHARED ${_hdr} ${_src})
generate_export_header(
  Plugin_PUJ_Ogre_DotXXSceneLoader
  EXPORT_MACRO_NAME Plugin_PUJ_Ogre_DotXXSceneLoader_EXPORT
  EXPORT_FILE_NAME PUJ_Ogre/Plugin_PUJ_Ogre_DotXXSceneLoader_Export.h
  )
target_link_libraries(
  Plugin_PUJ_Ogre_DotXXSceneLoader
  PUBLIC OgreMain ${VTK_LIBRARIES}
  PRIVATE pugixml
  )

## eof - CMakeLists.txt
