## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

option(${PROJECT_NAME}_build_bash "Build bash commands?" OFF)

include_directories(
  ${PROJECT_SOURCE_DIR}/lib
  ${PROJECT_BINARY_DIR}/lib
  )

if(${PROJECT_NAME}_build_bash)
  add_executable(PUJ_Ogre_bash_SceneViewer SceneViewer.cxx)
  target_link_libraries(PUJ_Ogre_bash_SceneViewer PUJ_Ogre)
endif()

## eof - CMakeLists.txt
