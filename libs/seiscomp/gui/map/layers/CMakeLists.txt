SET(GUI_MAP_LAYER_SOURCES
	annotationlayer.cpp
	citieslayer.cpp
	gridlayer.cpp
	geofeaturelayer.cpp
	symbollayer.cpp
)

SET(GUI_MAP_LAYER_HEADERS
	annotationlayer.h
	citieslayer.h
	gridlayer.h
	symbollayer.h
)

SET(GUI_MAP_LAYER_MOC_HEADERS
	geofeaturelayer.h
)

SC_SETUP_GUI_LIB_SUBDIR(GUI_MAP_LAYER)
