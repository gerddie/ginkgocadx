CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(BUILD_ALL)

SET( GINKGO_VERSION 3.7.2 CACHE STRING "Mayor version number")
SET( GINKGO_REVISION 1 CACHE STRING "Incremental revision number")
SET( GINKGO_BUILD 1 CACHE STRING "Incremental build number")

SET( USE_PATCHED_LIBS        FALSE CACHE BOOL "Use patched libraries")
SET( BUILD_VISUALIZATION_EXT TRUE CACHE BOOL "Build Visualization extention")

SET( USE_SYSTEM_SQLITE       TRUE CACHE BOOL "Build with system SQLite library")
SET( USE_CUSTOM_WX           FALSE CACHE BOOL "Build with custom WX library")
SET( USE_CUSTOM_VTK          FALSE CACHE BOOL "Build with custom VTK library")
SET( USE_CUSTOM_ITK          FALSE CACHE BOOL "Build with custom ITK library")
SET( USE_CUSTOM_DCMTK        FALSE CACHE BOOL "Build with custom DCMTK library")

SET( CUSTOM_PACKAGE          FALSE CACHE BOOL "Custom package")
SET( FORCE_SSE               FALSE CACHE BOOL "Force SSE Flag")
SET( INTERNET_DIST           TRUE CACHE BOOL "Public distribution")

SET( CMAKE_OSX_ARCHITECTURES "i386" CACHE STRING "MacOSX Architectures" FORCE)

IF(NOT CMAKE_BUILD_TYPE)
SET(CMAKE_BUILD_TYPE Release CACHE STRING
"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)


SET( GINKGO_VERSION_STR ${GINKGO_VERSION}.${GINKGO_REVISION}.${GINKGO_BUILD})

#############################################################################################

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
INCLUDE(SysDefs)

#############################################################################################

#doesn't work... using custom build
#IF(LINUX)
#FIND_PACKAGE(LIBCURL REQUIRED)
#ENDIF()
SET_CUSTOM_libcurl_PACKAGE()

IF(LINUX)
FIND_PACKAGE(GTK2 REQUIRED)
ENDIF()

IF(USE_PATCHED_LIBS)
MESSAGE("Using patched libs")
ENDIF()

IF(NOT USE_CUSTOM_WX)
MESSAGE("Using system wxWidgets")
FIND_PACKAGE(wxWidgets 3.0.1 COMPONENTS core base html gl net xml aui adv richtext xrc propgrid REQUIRED)
ELSE()
MESSAGE("Using custom wxWidgets")
SET_CUSTOM_wxWidgets_PACKAGE()
ENDIF()

IF(NOT USE_CUSTOM_VTK)
MESSAGE("Using system VTK")
FIND_PACKAGE(VTK REQUIRED)
ELSE()
MESSAGE("Using custom VTK")
SET_CUSTOM_VTK_PACKAGE()
ENDIF()

IF(NOT USE_CUSTOM_ITK)
MESSAGE("Using system ITK")
FIND_PACKAGE(ITK REQUIRED)
ELSE()
MESSAGE("Using custom ITK")
SET_CUSTOM_ITK_PACKAGE()
ENDIF()

IF(NOT USE_CUSTOM_DCMTK)
MESSAGE("Using system DCMTK")
FIND_PACKAGE(DCMTK 3.6.0 EXACT REQUIRED)
ELSE()
MESSAGE("Using custom DCMTK")
SET_CUSTOM_DCMTK_PACKAGE()
ENDIF()

IF(NOT USE_SYSTEM_SQLITE)
MESSAGE("Using builtin SQLite")
ELSE()
MESSAGE("Using system SQLite")
#FIND_PACKAGE(sqlite REQUIRED)

ENDIF()

#############################################################################################

IF(MACOS)
SET(GINKGO_PROJECT "Ginkgo CADx" CACHE PARENT_SCOPE "")
ELSEIF(LINUX AND CUSTOM_PACKAGE)
SET(GINKGO_PROJECT "ginkgocadx-${ARCH}" CACHE PARENT_SCOPE "")
ELSE()
SET(GINKGO_PROJECT "ginkgocadx" CACHE PARENT_SCOPE "")
ENDIF()

SET(GINKGO_PLUGINS "" CACHE PARENT_SCOPE "")

ADD_SUBDIRECTORY(cadxcore)
ADD_SUBDIRECTORY(ginkgocadx)

IF(BUILD_VISUALIZATION_EXT)
ADD_SUBDIRECTORY(visualizator)
ENDIF()

ADD_CUSTOM_TARGET(${PROJECT_NAME} echo)

MESSAGE("Building ${GINKGO_PROJECT} ${GINKGO_VERSION_STR}")

ADD_DEPENDENCIES(${PROJECT_NAME} "${GINKGO_PROJECT}")

IF(BUILD_VISUALIZATION_EXT)
ADD_DEPENDENCIES(${PROJECT_NAME} visualizator)
LIST(APPEND GINKGO_PLUGINS "visualizator")
ENDIF()

IF(CUSTOM_PACKAGE)

SET( DEST "${CMAKE_CURRENT_BINARY_DIR}/dist" )
PACKAGE_RULES( "${PROJECT_NAME}" "${ARCH}" "${DEST}" )
ENDIF()

