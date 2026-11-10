## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

add_executable(
  pujOgre_SolarSystem
  Application.cxx
  Model.cxx
  main.cxx
  )
target_link_libraries(
  pujOgre_SolarSystem
  PRIVATE OgreBites
  )

list(JOIN pujOgre_PLUGINS "|" _plugins)
target_compile_definitions(
  pujOgre_SolarSystem
  PRIVATE
  pujOgre_PLUGINS="${_plugins}"
  pujOgre_SHARED_EXT="${CMAKE_SHARED_LIBRARY_SUFFIX}"
  )

## eof - CMakeLists.txt
