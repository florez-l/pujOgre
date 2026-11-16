## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

include_directories(${PROJECT_BINARY_DIR}/lib ${PROJECT_SOURCE_DIR}/lib)
add_executable(pujOgre_SceneViewer main.cxx)
target_link_libraries(pujOgre_SceneViewer PRIVATE pujOgre)

## eof - CMakeLists.txt
