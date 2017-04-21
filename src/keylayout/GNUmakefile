# Makefile for Key Layout File Compiler
#
CC = gcc
CFLAGS = -O3 -MMD
INCLUDES = -I. -I..
DEFINES = -DPLATFORM_WINDOWS -DDEBUG_YACC
CFLAGS += $(INCLUDES) $(DEFINES)

TARGET = klfc

OBJPATH = obj/
OBJS = \
	$(OBJPATH)main.o \
	$(OBJPATH)klfcompiler_lex.o \
	$(OBJPATH)klfcompiler.tab.o

DEPENDS	= $(subst .o,.d,$(OBJS))

compile: mk_dir $(TARGET)

mk_dir:
	@mkdir -p $(OBJPATH)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
#	@rm -f klfcompiler_lex.c klfcompiler.tab.h klfcompiler.tab.c
	@rm -f $(OBJS)
	@rm -f $(TARGET)
	@rm -f $(DEPENDS)
	@rm -f *.klx
	@rm -f $(OBJPATH)*.obj

yet:
	@grep -n Yet *.h *.cpp

$(OBJPATH)%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


klfcompiler_lex.c: klfcompiler.l
	flex -o $@ klfcompiler.l

klfcompiler.tab.h: klfcompiler.y
	bison -d klfcompiler.y

klfcompiler.tab.c: klfcompiler.y
	bison klfcompiler.y

-include $(DEPENDS)

.SUFFIXES: .c .h .o .d .l
.PHONY: compile clean mk_dir yet
