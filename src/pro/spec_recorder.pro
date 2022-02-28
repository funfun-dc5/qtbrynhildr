# recorder
recorder {
DEFINES += QTB_RECORDER=1
HEADERS += function/recorder.h
SOURCES += function/recorder.cpp
}
else {
DEFINES += QTB_RECORDER=0
}
