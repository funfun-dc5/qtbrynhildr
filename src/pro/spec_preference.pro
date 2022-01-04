# preference
preference {
DEFINES += QTB_PREFERENCE=1
FORMS += GUI/preference_dialog.ui
HEADERS += dialog/preference_dialog.h
SOURCES += dialog/preference_dialog.cpp
}
else {
DEFINES += QTB_PREFERENCE=0
}
