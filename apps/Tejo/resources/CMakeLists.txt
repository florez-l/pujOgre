## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

set(
  _files
  cancha.obj
  sphere.obj
  material.material
  tejo.scenexx
  )

add_custom_target(
  PUJ_Ogre_Tejo_resources
  ALL
  COMMAND
  ${CMAKE_COMMAND} -E tar cvf
  ${PROJECT_BINARY_DIR}/PUJ_Ogre_Tejo_resources.zip --format=zip
  ${_files}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg.in
  ${PROJECT_BINARY_DIR}/PUJ_Ogre_Tejo_resources.cfg
  @ONLY
  )

## eof - CMakeLists.txt
