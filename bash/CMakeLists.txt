## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

option(${PROJECT_NAME}_build_bash "Build bash commands?" OFF)

include_directories(
  ${BULLET_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/lib
  ${PROJECT_BINARY_DIR}/lib
  )

if(${PROJECT_NAME}_build_bash)
  add_executable(PUJ_Ogre_bash_ParametricViewer ParametricViewer.cxx)
  target_link_libraries(
    PUJ_Ogre_bash_ParametricViewer
    OgreMain OgreBites OgreBullet
    )
endif()

## eof - CMakeLists.txt
