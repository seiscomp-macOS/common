SUBDIRS(seiscomp plugins share)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${SC3_PACKAGE_SOURCE_DIR}/libs/3rd-party)

SET(WIRED_APP_TARGET scmaster)
SET(WIRED_APP_SOURCES
	protocols/http.cpp
	protocols/websocket.cpp
	protocols/handler/broker.cpp
	protocols/handler/db.cpp
	statistics/statistics.cpp
	main.cpp
	server.cpp
	settings.cpp
)

SC_ADD_EXECUTABLE(WIRED_APP ${WIRED_APP_TARGET})
SC_LINK_LIBRARIES_INTERNAL(${WIRED_APP_TARGET} broker)
SC_INSTALL_INIT(${WIRED_APP_TARGET} config/scmaster.py)

FILE(GLOB descs "${CMAKE_CURRENT_SOURCE_DIR}/descriptions/*.xml")
INSTALL(FILES ${descs} DESTINATION ${SC3_PACKAGE_APP_DESC_DIR})
