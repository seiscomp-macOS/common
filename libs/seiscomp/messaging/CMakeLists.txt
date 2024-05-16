SET(MSG_SOURCES
	connection.cpp
	packet.cpp
	protocol.cpp
	status.cpp
	protocols/scmp/socket.cpp
	protocols/scmp/websocket.cpp
)

SET(MSG_HEADERS
	connection.h
	packet.h
	protocol.h
	status.h
)

IF(APPLE)
    FIND_PACKAGE(OpenSSL REQUIRED)
    INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
ENDIF(APPLE)

SC_ADD_SUBDIR_SOURCES(MSG database)
SC_ADD_SUBDIR_SOURCES(MSG messages)
SC_SETUP_LIB_SUBDIR(MSG)
