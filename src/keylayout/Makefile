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
	klfcompiler_lex.c \
	klfcompiler.tab.c

OBJS = \
	obj\main.obj \
	obj\klfcompiler_lex.obj \
	obj\klfcompiler.tab.obj

.SUFFIXES: .c .h

compile: $(TARGET)

$(TARGET): $(OBJS)
	$(LINKER) $(LFLAGS) $(OBJS) /OUT:$(TARGET)

obj\main.obj: main.c
	$(CC) -c $(CFLAGS) -Foobj\ main.c

obj\klfcompiler_lex.obj: klfcompiler_lex.c
	$(CC) -c $(CFLAGS) -Foobj\ klfcompiler_lex.c

obj\klfcompiler.tab.obj: klfcompiler.tab.c
	$(CC) -c $(CFLAGS) -Foobj\ klfcompiler.tab.c

clean:
	@del $(OBJS)
	@del $(TARGET)
	@del *.klx
