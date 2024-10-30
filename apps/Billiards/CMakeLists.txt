## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

add_executable(PUJ_Ogre_Billiards main.cxx)
target_link_libraries(PUJ_Ogre_Billiards PUJ_Ogre)

subdirs(resources)

## eof - CMakeLists.txt
