## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/Config.h
  @ONLY
  )

include_directories(
  ${BULLET_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}/lib
  )
set(
  _hdr
  ${CMAKE_CURRENT_BINARY_DIR}/Config.h
  Application.h
  ApplicationWithBullet.h
  #  BaseApplicationWithBullet.h
  Export.h
  )
set(
  _src
  Application.cxx
  ApplicationWithBullet.cxx
  #  BaseApplicationWithBullet.cxx
  #  OBJReader.cxx
  #  OgreBullet.cxx
  )

add_library(PUJ_Ogre SHARED "${_hdr};${_src}")
generate_export_header(
  PUJ_Ogre
  EXPORT_MACRO_NAME PUJ_Ogre_EXPORT
  EXPORT_FILE_NAME Export.h
  )
target_link_libraries(
  PUJ_Ogre
  OgreMain OgreBites OgreBullet
  )

## eof - CMakeLists.txt
