CC = gcc
CFLAGS = -O3 -MMD
INCLUDES = -I. -I..
DEFINES =
CFLAGS += $(INCLUDES) $(DEFINES)

TARGET = keylayoutfile

OBJPATH = obj/
OBJS = \
	$(OBJPATH)keylayoutfile.o \
	$(OBJPATH)keylayoutfile.tab.o

DEPENDS	= $(subst .o,.d,$(OBJS))

compile: mk_dir $(TARGET)

mk_dir:
	@mkdir -p $(OBJPATH)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	@rm -f keylayoutfile.c keylayoutfile.tab.h keylayoutfile.tab.c
	@rm -f $(OBJS)
	@rm -f $(TARGET)
	@rm -f $(DEPENDS)
	@rm -f TEST.klx

yet:
	@grep -n Yet *.h *.cpp

$(OBJPATH)%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


keylayoutfile.c: keylayoutfile.l keylayoutfile.tab.h
	flex -o $@ keylayoutfile.l

keylayoutfile.tab.h: keylayoutfile.y
	bison -d keylayoutfile.y

keylayoutfile.tab.c: keylayoutfile.y
	bison keylayoutfile.y

-include $(DEPENDS)

.SUFFIXES: .c .h .o .d .l
.PHONY: compile clean mk_dir yet
