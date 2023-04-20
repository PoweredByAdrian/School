# Install script for directory: C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/Debug/SDL2maind.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/Release/SDL2main.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/MinSizeRel/SDL2mains.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/RelWithDebInfo/SDL2mainrd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/Debug/SDL2d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/Release/SDL2.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/MinSizeRel/SDL2s.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/RelWithDebInfo/SDL2rd.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2mainTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2mainTargets.cmake"
         "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2mainTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2mainTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2mainTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2mainTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2mainTargets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2mainTargets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2mainTargets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2mainTargets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2staticTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2staticTargets.cmake"
         "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2staticTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2staticTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2staticTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2staticTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2staticTargets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2staticTargets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2staticTargets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/CMakeFiles/Export/cmake/SDL2staticTargets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/SDL2Config.cmake"
    "C:/Programy/SKOLA/IZG/izgProject/build/SDL2ConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_assert.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_atomic.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_audio.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_bits.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_blendmode.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_clipboard.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_android.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_emscripten.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_iphoneos.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_macosx.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_minimal.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_os2.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_pandora.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_psp.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_windows.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_winrt.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_config_wiz.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_copying.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_cpuinfo.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_egl.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_endian.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_error.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_events.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_filesystem.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_gamecontroller.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_gesture.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_haptic.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_hidapi.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_hints.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_joystick.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_keyboard.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_keycode.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_loadso.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_locale.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_log.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_main.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_messagebox.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_metal.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_misc.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_mouse.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_mutex.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_name.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_opengl.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_opengl_glext.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_opengles.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_opengles2.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_opengles2_gl2.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_opengles2_gl2ext.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_opengles2_gl2platform.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_opengles2_khrplatform.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_pixels.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_platform.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_power.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_quit.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_rect.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_render.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_rwops.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_scancode.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_sensor.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_shape.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_stdinc.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_surface.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_system.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_syswm.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_assert.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_common.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_compare.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_crc32.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_font.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_fuzzer.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_harness.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_images.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_log.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_md5.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_memory.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_test_random.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_thread.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_timer.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_touch.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_types.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_version.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_video.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/SDL_vulkan.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/begin_code.h"
    "C:/Programy/SKOLA/IZG/izgProject/libs/SDL2-2.0.20/include/close_code.h"
    "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/include/SDL_config.h"
    "C:/Programy/SKOLA/IZG/izgProject/build/libs/SDL2-2.0.20/include/SDL_revision.h"
    )
endif()

