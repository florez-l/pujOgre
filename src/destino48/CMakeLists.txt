## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

if(DEFINED destino48_RESOURCES_DIR)

  # - Headers paths
  include_directories(
    ${PROJECT_BINARY_DIR}/lib
    ${PROJECT_SOURCE_DIR}/lib
    )

  # Prepare resources
  set(_resources_dirs "./destino48_resources")
  file(
    GLOB_RECURSE _resources
    LIST_DIRECTORIES true
    "${destino48_RESOURCES_DIR}/*"
    )
  foreach(_r ${_resources})
    if(IS_DIRECTORY "${_r}")
      string(REGEX REPLACE "^${destino48_RESOURCES_DIR}" "" _s "${_r}")
      list(APPEND _resources_dirs "./destino48_resources${_s}")
    endif()
  endforeach()

  set(_config "[General]")
  foreach(_d ${_resources_dirs})
    string(APPEND _config "\nFileSystem=${_d}")
  endforeach()

  set(_o "${PROJECT_BINARY_DIR}/destino48_resources.cfg")
  file(GENERATE OUTPUT "${_o}" CONTENT "${_config}")
  add_custom_target(
    destino48_CREATE_RESOURCES_FILE
    ALL
    COMMAND ""
    COMMENT "Writing string to ${_o}"
    VERBATIM
    )

  add_executable(destino48 main.cxx)
  add_dependencies(destino48 destino48_CREATE_RESOURCES_FILE)
  target_link_libraries(destino48 PUBLIC pujOgre)

  # Copy contents of 'my_data_dir' from source to the target's binary directory
  add_custom_command(
    TARGET destino48
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${destino48_RESOURCES_DIR}
    $<TARGET_FILE_DIR:destino48>/destino48_resources
    COMMENT "Copying data directory for destino48"
    )
  
  # Installation rules
  install(
    TARGETS destino48
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
    )

else()
  message(
    STATUS "destino48_RESOURCES_DIR not defined, not building destino48"
    )
endif()

## eof - CMakeLists.txt
