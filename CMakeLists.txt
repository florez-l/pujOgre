## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

cmake_minimum_required(VERSION 3.20)
project(pujOgre VERSION 0.0.0.1)

# Base configurations
include(cmake/pujOgreBase.cmake)

# Targets
subdirs(src)

# Installation directives
include(cmake/pujOgreInstall.cmake)

## eof - CMakeLists.txt
