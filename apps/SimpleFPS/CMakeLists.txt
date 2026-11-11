## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

include_directories(
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}/lib
  )
add_executable(
  pujOgre_SimpleFPS
  Application.cxx
  Model.cxx
  main.cxx
  )
target_link_libraries(
  pujOgre_SimpleFPS
  PRIVATE OgreBites OgreBullet pujOgre
  )

list(JOIN pujOgre_PLUGINS "|" _plugins)
target_compile_definitions(
  pujOgre_SimpleFPS
  PRIVATE
  pujOgre_PLUGINS="${_plugins}"
  pujOgre_SHARED_EXT="${CMAKE_SHARED_LIBRARY_SUFFIX}"
  )

add_custom_target(
  pujOgre_SimpleFPS_copy_resources ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory
  ${CMAKE_BINARY_DIR}/SimpleFPS_resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/resources
  ${PROJECT_BINARY_DIR}/SimpleFPS_resources
  COMMENT "Copying SimpleFPS resources"
)
add_dependencies(pujOgre_SimpleFPS pujOgre_SimpleFPS_copy_resources)

## eof - CMakeLists.txt
