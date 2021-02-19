#---------------------------------------------------------------------------
# makefile with GNU make & gcc
#---------------------------------------------------------------------------

CC		= g++
INCS	= -I.
CPPFLAGS= -O3 -MMD -fno-builtin
HOST	= mingw81-x64

# for debug
DFLAGS	=

OBJPATH = obj/$(HOST)/
LIBPATH = $(HOST)/

TARGET	= $(LIBPATH)libbfsh.a

OBJS	= \
$(OBJPATH)blowfish.o

DEPENDS	= $(subst .o,.d,$(OBJS))

default: compile

-include $(DEPENDS)

#############################################################
compile: mk_dir	$(TARGET)

$(TARGET):	$(OBJS)
	ar cru $(TARGET) $(OBJS)
	ranlib $(TARGET)

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJS)
	@rm -f $(OBJPATH)*.d

mk_dir:
	@mkdir -p $(OBJPATH) $(LIBPATH)

#############################################################
$(OBJPATH)%.o: %.cpp
	$(CC) -c $(CPPFLAGS) $(DFLAGS) $(INCS) -o $@ $<

.SUFFIXES: .cpp .h .o .d
.PHONY: default compile clean
