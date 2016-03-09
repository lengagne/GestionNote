message(STATUS ${CMAKE_CURRENT_LIST_FILE})

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif (NOT MSVC)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

message("GestionNote_SOURCE_DIR = " ${GestionNote_SOURCE_DIR})

include_directories(${GestionNote_SOURCE_DIR})
include_directories(${GestionNote_SOURCE_DIR}/zlib)
link_directories(${GestionNote_BINARY_DIR})
link_directories(${GestionNote_BINARY_DIR}/lib)


find_package(Qt5Core REQUIRED)

# set all include directories for in and out of source builds
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${ZLIB_INCLUDE_DIRS}
)

file(GLOB_RECURSE src_files
	"*.h" "*.hh" "*.hpp" "*.hxx"
	"*.c" "*.cc" "*.cpp")

if (DEFINED QUAZIP_STATIC)
	set(lib_type STATIC)
else (DEFINED QUAZIP_STATIC)
	set(lib_type SHARED)
	add_definitions(-DQUAZIP_SHARED -DQUAZIP_BUILD)
endif (DEFINED QUAZIP_STATIC)

add_library(quazip SHARED ${lib_type} ${src_files})
#set_target_properties(quazip PROPERTIES VERSION 1.0.0 SOVERSION 1)
target_link_libraries(quazip Qt5::Core zlib)

install( TARGETS quazip LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)
