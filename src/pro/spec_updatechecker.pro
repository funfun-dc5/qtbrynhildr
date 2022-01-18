# update checker
updatecheck {
DEFINES += QTB_UPDATECHECK=1
HEADERS += util/httpgetter.h
SOURCES += util/httpgetter.cpp
}
else {
DEFINES += QTB_UPDATECHECK=0
}
