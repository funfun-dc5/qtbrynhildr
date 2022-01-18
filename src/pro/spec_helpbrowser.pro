# help browser
help_browser {
HEADERS += util/helpbrowser.h
SOURCES += util/helpbrowser.cpp
DEFINES += QTB_HELP_BROWSER=1
}
else {
DEFINES += QTB_HELP_BROWSER=0
}
