# CELT
celt {
	DEFINES += QTB_CELT_SUPPORT=1
	INCLUDEPATH += ../libs/celt/include
	HEADERS += soundthread/converter.h soundthread/converter_celt.h
	SOURCES += soundthread/converter.cpp soundthread/converter_celt.cpp
	LIBS += -L../libs/celt/$$BUILDARCH -lcelt
}
else {
	DEFINES += QTB_CELT_SUPPORT=0
}
