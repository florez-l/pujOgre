## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

add_executable(pujOgre_SimpleParametricExpressionViewer main.cxx)
target_link_libraries(
  pujOgre_SimpleParametricExpressionViewer
  PRIVATE OgreBites
  )
target_compile_definitions(
  pujOgre_SimpleParametricExpressionViewer
  PRIVATE MEDIA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/media"
  )

# ---------------------------------------------------------------------------
# plugins.cfg, written next to the executable
# ---------------------------------------------------------------------------
# Plugins from Ogre's plugin folder (OGRE_PLUGIN_DIR comes from find_package(OGRE)).
# Only the ones that actually exist are enabled, so a missing render system
# doesn't stop the app from starting.
set(APP_PLUGINS
    RenderSystem_GL3Plus
    RenderSystem_GL
    RenderSystem_Vulkan
    RenderSystem_Direct3D11
    Codec_STBI
    Plugin_DotScene
    CACHE STRING "Plugins to load from OGRE_PLUGIN_DIR")

# Plugins built elsewhere, as full paths, e.g.
#   -DAPP_EXTRA_PLUGINS="/home/me/vtkcodec/build/Plugin_VtkCodec.so"
set(
  APP_EXTRA_PLUGINS
  ""
  CACHE STRING "Full paths of plugins outside OGRE_PLUGIN_DIR (;-separated)"
  )

set(APP_PLUGIN_LINES "")
foreach(plugin IN LISTS APP_PLUGINS)
    if(EXISTS "${OGRE_PLUGIN_DIR}/${plugin}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        string(APPEND APP_PLUGIN_LINES "Plugin=${plugin}\n")
    else()
        string(APPEND APP_PLUGIN_LINES "# Plugin=${plugin}   (not found in ${OGRE_PLUGIN_DIR})\n")
    endif()
endforeach()

set(APP_EXTRA_PLUGIN_LINES "")
foreach(plugin IN LISTS APP_EXTRA_PLUGINS)
    string(APPEND APP_EXTRA_PLUGIN_LINES "ExtraPlugin=${plugin}\n")
endforeach()

configure_file(plugins.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg.configured @ONLY)
# file(GENERATE) puts it next to the executable, also for multi-config generators (Debug/, Release/)
file(GENERATE OUTPUT "$<TARGET_FILE_DIR:pujOgre_SimpleParametricExpressionViewer>/plugins.cfg"
  INPUT  "${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg.configured")

## eof - CMakeLists.txt
