# CMakeList.txt : CMake project for main, include source and define
# project specific logic here.
#
# author Matúš Vráblik (xvrabl05)
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("main")

# C++ 17 required standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QT inclusion
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# Find Qt package
find_package(Qt5 REQUIRED COMPONENTS Widgets Core OpenGL Xml)

# Include our code
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/Forms")
set(SOURCES_DIR "${CMAKE_SOURCE_DIR}/src")
set(HEADERS_DIR "${CMAKE_SOURCE_DIR}/Headers")
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/Resources")

file(GLOB_RECURSE SOURCES
	"${SOURCES_DIR}/*.cpp"
	"${HEADERS_DIR}/*.h"
	"${FORMS_DIR}/*.ui"
	"${RESOURCES_DIR}/*"
)

# Add forms to AUTOIC search path
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOIC_SEARCH_PATHS} ${FORMS_DIR})

# Add source to this project's executable.
add_executable (pacman ${SOURCES})
target_include_directories(pacman PRIVATE ${FORMS_DIR} PRIVATE ${HEADERS_DIR} PRIVATE ${SOURCES_DIR} PRIVATE ${RESOURCES_DIR})
target_link_libraries(pacman PRIVATE Qt5::Widgets)
set_target_properties(pacman PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)