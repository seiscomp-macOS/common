SET(AMPS_SOURCES
	a5_2.cpp
	idc_utils.cpp
	mBc.cpp
	mBc_measure.cpp
	mb.cpp
	m_B.cpp
	Mjma.cpp
	ML.cpp
	MLc.cpp
	MLh.cpp
	MLv.cpp
	msbb.cpp
	Ms20.cpp
	Mwp.cpp
	sbsnr.cpp
	iaspei.cpp
)

SET(AMPS_HEADERS
	mBc.h
	mBc_measure.h
	mb.h
	m_B.h
	Mjma.h
	ML.h
	MLc.h
	MLh.h
	MLv.h
	msbb.h
	Ms20.h
	Mwp.h
	iaspei.h
)

SC_SETUP_LIB_SUBDIR(AMPS)
