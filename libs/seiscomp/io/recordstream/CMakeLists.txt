SET(RECORDSTREAM_SOURCES
	file.cpp
	memory.cpp
	sdsarchive.cpp
	arclink.cpp
	slconnection.cpp
	combined.cpp
	concurrent.cpp
	balanced.cpp
	routing.cpp
	streamidx.cpp
	decimation.cpp
	resample.cpp
	fdsnws.cpp
	httpmsgbus.cpp
	caps.cpp
	caps/packet.cpp
	caps/anypacket.cpp
	caps/rawpacket.cpp
	caps/riff.cpp
	caps/sessiontable.cpp
	remez/remez.c
)

SET(RECORDSTREAM_HEADERS
	file.h
	memory.h
	archive.h
	sdsarchive.h
	arclink.h
	slconnection.h
	combined.h
	concurrent.h
	streamidx.h
	decimation.h
	resample.h
	fdsnws.h
	httpmsgbus.h
)

SC_SETUP_LIB_SUBDIR(RECORDSTREAM)
