## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

set(
  _hdr
  BaseApplication.h
  BaseListener.h
  )

set(
  _src
  BaseApplication.cxx
  BaseListener.cxx
  )
set(_lib OgreBites)

# -- Headers paths
include_directories(
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}/lib
  )

# -- Build library
add_library(pujOgre ${pujOgre_BUILD_TYPE} ${_src})
generate_export_header(
  pujOgre
  EXPORT_MACRO_NAME pujOgre_EXPORT
  EXPORT_FILE_NAME Export.h
  )
target_link_libraries(pujOgre PUBLIC ${_lib})

# -- Installation rules
install(
  TARGETS pujOgre
  EXPORT "${targets_export_name}"
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
  RUNTIME DESTINATION "bin"
  INCLUDES DESTINATION "${include_install_dir}"
  )

foreach(_h ${_hdr})
  install(FILES ${_h} DESTINATION include/pujOgre/${_d})
endforeach(_h)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Export.h DESTINATION include/pujOgre)

## eof - CMakeLists.txt
