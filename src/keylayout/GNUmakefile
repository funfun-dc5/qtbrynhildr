CC = gcc
CFLAGS = -O3 -MMD
INCLUDES = -I.
DEFINES =
CFLAGS += $(INCLUDES) $(DEFINES)

TARGET = langfile

OBJPATH = obj/
OBJS = \
	$(OBJPATH)langfile.o \

DEPENDS	= $(subst .o,.d,$(OBJS))

compile: mk_dir $(TARGET)

mk_dir:
	@mkdir -p $(OBJPATH)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	@rm -f langfile.c
	@rm -f $(OBJS)
	@rm -f $(TARGET)
	@rm -f $(DEPENDS)

yet:
	@grep -n Yet *.h *.cpp

$(OBJPATH)%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


langfile.c:
	flex -o $@ langfile.lex

-include $(DEPENDS)

.SUFFIXES: .c .h .o .d .lex
.PHONY: compile clean mk_dir yet
