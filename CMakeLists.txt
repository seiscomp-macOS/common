SC_BEGIN_PACKAGE(COMMON)

OPTION(SC_TRUNK_DB_MYSQL "Add MYSQL support" ON)
OPTION(SC_TRUNK_DB_SQLITE3 "Add SQLite3 support" OFF)
OPTION(SC_TRUNK_DB_POSTGRESQL "Add PostgreSQL support" OFF)

SET(PROJECT_TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test/data)

#SET(Boost_DEBUG ON)

# Static Boost libs are not compiled with -fPIC on some
# systems so linking into a 64bit shared library does not work.
# Disabled it so far.
#SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_DETAILED_FAILURE_MSG ON)
FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options thread filesystem iostreams regex container)
IF (WIN32)
	ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")
	ADD_DEFINITIONS("-DBOOST_LIB_DIAGNOSTIC")
ENDIF (WIN32)

FIND_PACKAGE(OpenSSL REQUIRED)

# On macOS search for bson-v1.0 dylib with pkgconf, which is done in common/libs/seiscomp/CMakeLists.txt
# Once BSON_LINK_LIBRARIES are found, add it to macro SC_LIB_LINK_LIBRARIES() in /libs/seiscomp/CMakeLists.txt
# bson-1.0 comes with Homebrew "mongo-c-driver@1"
# If you link bson-v1.0 dylib from BSON_LINK_LIBRARIES here it fails to compile lib/libseiscomp_core.dylib
# bson-v1.0 dylib needs to be linked in to macro SC_LIB_LINK_LIBRARIES() in /libs/seiscomp/CMakeLists.txt
IF(NOT APPLE)
    FIND_PACKAGE(bson-1.0 1.14 REQUIRED)
ENDIF(NOT APPLE)


IF (SC_TRUNK_DB_MYSQL)
	FIND_PACKAGE(MySQL REQUIRED)
ENDIF (SC_TRUNK_DB_MYSQL)

IF (SC_TRUNK_DB_SQLITE3)
	FIND_PACKAGE(SQLite3 REQUIRED)
ENDIF (SC_TRUNK_DB_SQLITE3)

IF (SC_TRUNK_DB_POSTGRESQL)
	FIND_PACKAGE(PostgreSQL REQUIRED)
ENDIF (SC_TRUNK_DB_POSTGRESQL)

SET(THIRD_PARTY_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/3rd-party)
INCLUDE_DIRECTORIES(${THIRD_PARTY_DIRECTORY})

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libs)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/libs)

SUBDIRS(apps libs plugins)
