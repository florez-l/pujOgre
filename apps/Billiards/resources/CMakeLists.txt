## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

set(
  _files
  billiards.scenexx
  materials.material
  red_ball.mesh
  Table0.mesh
  Table1.mesh
  Table2.mesh
  Table3.mesh
  Table4.mesh
  white_ball.mesh
  yellow_ball.mesh
  )

add_custom_target(
  PUJ_Ogre_Billiards_resources
  ALL
  COMMAND
  ${CMAKE_COMMAND} -E tar cvf
  ${PROJECT_BINARY_DIR}/PUJ_Ogre_Billiards_resources.zip --format=zip
  ${_files}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg.in
  ${PROJECT_BINARY_DIR}/PUJ_Ogre_Billiards_resources.cfg
  @ONLY
  )

## eof - CMakeLists.txt
