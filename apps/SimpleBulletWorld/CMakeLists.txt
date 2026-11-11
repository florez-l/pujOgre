## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

add_executable(
  pujOgre_SimpleBulletWorld
  Application.cxx
  Model.cxx
  main.cxx
  )
target_link_libraries(
  pujOgre_SimpleBulletWorld
  PRIVATE OgreBites OgreBullet
  )

list(JOIN pujOgre_PLUGINS "|" _plugins)
target_compile_definitions(
  pujOgre_SimpleBulletWorld
  PRIVATE
  pujOgre_PLUGINS="${_plugins}"
  pujOgre_SHARED_EXT="${CMAKE_SHARED_LIBRARY_SUFFIX}"
  )

## eof - CMakeLists.txt
