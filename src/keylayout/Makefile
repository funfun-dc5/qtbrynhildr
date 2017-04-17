# Makefile (nmake) for Key Layout File Compiler
#
CC = cl
INCLUDES = -I. -I..
DEFINES = -DPLATFORM_WINDOWS -DYY_NO_UNISTD_H
CFLAGS = $(INCLUDES) $(DEFINES)
LINKER= link
LFLAGS=

TARGET = klfc.exe

SOURCES = \
	main.c \
	klfcompiler.c \
	klfcompiler.tab.c

OBJS = \
	obj\main.obj \
	obj\klfcompiler.obj \
	obj\klfcompiler.tab.obj

.SUFFIXES: .c .h

compile: $(TARGET)

$(TARGET): $(OBJS)
	$(LINKER) $(LFLAGS) $(OBJS) /OUT:$(TARGET)

obj\main.obj: main.c
	$(CC) -c $(CFLAGS) -Foobj\ main.c

obj\klfcompiler.obj: klfcompiler.c
	$(CC) -c $(CFLAGS) -Foobj\ klfcompiler.c

obj\klfcompiler.tab.obj: klfcompiler.tab.c
	$(CC) -c $(CFLAGS) -Foobj\ klfcompiler.tab.c

clean:
	@del $(OBJS)
	@del $(TARGET)
	@del *.klx
