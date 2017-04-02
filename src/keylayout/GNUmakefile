CC = gcc
CFLAGS = -O3 -MMD
INCLUDES = -I.
DEFINES =
CFLAGS += $(INCLUDES) $(DEFINES)

TARGET = keylayoutfile

OBJPATH = obj/
OBJS = \
	$(OBJPATH)keylayoutfile.o \

DEPENDS	= $(subst .o,.d,$(OBJS))

compile: mk_dir $(TARGET)

mk_dir:
	@mkdir -p $(OBJPATH)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	@rm -f keylayoutfile.c
	@rm -f $(OBJS)
	@rm -f $(TARGET)
	@rm -f $(DEPENDS)

yet:
	@grep -n Yet *.h *.cpp

$(OBJPATH)%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


keylayoutfile.c:
	flex -o $@ keylayoutfile.l

-include $(DEPENDS)

.SUFFIXES: .c .h .o .d .l
.PHONY: compile clean mk_dir yet
