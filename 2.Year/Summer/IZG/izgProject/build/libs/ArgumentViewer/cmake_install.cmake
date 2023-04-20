# Install script for directory: C:/Programy/SKOLA/IZG/izgProject/libs/ArgumentViewer

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/izgProject")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/Debug/ArgumentViewerd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/Release/ArgumentViewer.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/MinSizeRel/ArgumentViewers.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/RelWithDebInfo/ArgumentViewerrd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ArgumentViewer" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/ArgumentViewer/argumentviewer_export.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ArgumentViewer" TYPE FILE FILES
    "C:/Programy/SKOLA/IZG/izgProject/libs/ArgumentViewer/src/ArgumentViewer/ArgumentViewer.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/ArgumentViewer/src/ArgumentViewer/Fwd.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/ArgumentViewer/src/ArgumentViewer/Exception.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer" TYPE FILE FILES
    "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/ArgumentViewer/ArgumentViewerConfig.cmake"
    "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/ArgumentViewer/ArgumentViewerConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer/ArgumentViewerTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer/ArgumentViewerTargets.cmake"
         "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/CMakeFiles/Export/lib/cmake/ArgumentViewer/ArgumentViewerTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer/ArgumentViewerTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer/ArgumentViewerTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/CMakeFiles/Export/lib/cmake/ArgumentViewer/ArgumentViewerTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/CMakeFiles/Export/lib/cmake/ArgumentViewer/ArgumentViewerTargets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/CMakeFiles/Export/lib/cmake/ArgumentViewer/ArgumentViewerTargets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/CMakeFiles/Export/lib/cmake/ArgumentViewer/ArgumentViewerTargets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/ArgumentViewer/CMakeFiles/Export/lib/cmake/ArgumentViewer/ArgumentViewerTargets-release.cmake")
  endif()
endif()

