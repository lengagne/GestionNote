#cmake -G "MinGW Makefiles"
message(STATUS "${CMAKE_CURRENT_LIST_FILE}")
cmake_minimum_required(VERSION 2.8.12)
Project(GestionNote)

set(LIBRARY_OUTPUT_PATH "${GestionNote_BINARY_DIR}/lib")

message(STATUS ${CMAKE_SYSTEM}) # print OS name

#set (BUILD_STATIC_LIBRARIES "1")
if (BUILD_STATIC_LIBRARIES)
	set(ODS_STATIC "1")
	set(QUAZIP_STATIC "1")
	set(ZLIB_STATIC "1")
endif (BUILD_STATIC_LIBRARIES)

SET(INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_INCLUDEDIR}/include/GestionNote)
SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )

add_subdirectory(zlib)
add_subdirectory(quazip)
add_subdirectory(ods)
add_subdirectory(GestionNote)
