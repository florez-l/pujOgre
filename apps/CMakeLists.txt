## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

include_directories(
  ${BULLET_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}/lib
  )

subdirs(Tejo)

## eof - CMakeLists.txt
