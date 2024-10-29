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

macos-x64 {
	LIBS -= -L../libs/celt/$$BUILDARCH -lcelt
	LIBS += -L../libs/celt/macos-x64 -lcelt_x64
}

macos-x64 {
	LIBS -= -L../libs/celt/$$BUILDARCH -lcelt
	LIBS += -L../libs/celt/macos-arm64 -lcelt_arm64
}
