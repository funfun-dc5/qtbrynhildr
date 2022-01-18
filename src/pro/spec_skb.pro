# software keyboard and button
software_keyboard_button {
CONFIG += keylayoutfile
DEFINES += QTB_SOFTWARE_KEYBOARD_AND_BUTTON=1
HEADERS += touchpanel2/software_keyboard.h touchpanel2/sk.h
SOURCES += touchpanel2/software_keyboard.cpp touchpanel2/sk.cpp
HEADERS += touchpanel2/software_button.h touchpanel2/sb.h
SOURCES += touchpanel2/software_button.cpp touchpanel2/sb.cpp
}
else {
DEFINES += QTB_SOFTWARE_KEYBOARD_AND_BUTTON=0
}

# keylayout file
keylayoutfile {
HEADERS += keylayout/keylayoutfile.h
SOURCES += keylayout/keylayoutfile.cpp
HEADERS += keylayout/keylayoutfilereader.h keylayout/keylayoutfilemanager.h
SOURCES += keylayout/keylayoutfilereader.cpp keylayout/keylayoutfilemanager.cpp
HEADERS += keylayout/klfcompiler.h keylayout/klfcompiler.tab.h
SOURCES += keylayout/klfcompiler_lex.c keylayout/klfcompiler.tab.c
DEFINES += USE_KEYLAYOUTFILE YY_NO_UNPUT YY_NO_INPUT
}
