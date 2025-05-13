CC ?= gcc
AR ?= ar
LD ?= ld

OBJDIR = build

SUBDIRS := colorUtils printfUtils printHexTable

LIB_COLORUTILS_TARGET := $(OBJDIR)/libcolorutils.a
LIB_PRINTFUTILS_TARGET := $(OBJDIR)/libprintfutils.a
LIB_PRINTHEXTABLE_TARGET := $(OBJDIR)/libprinthextable.a


export CC AR LD

.PHONY: all clean $(SUBDIRS)


LIB_COLORUTILS_OBJS := $(wildcard colorUtils/build/*.o)
LIB_PRINTFUTILS_OBJS := $(wildcard printfUtils/build/*.o)
LIB_PRINTHEXTABLE_OBJS :=\
												 printHexTable/build/printHexTable.o\
												 colorUtils/build/hsv.o\
												 colorUtils/build/ansi.o\
												 colorUtils/build/floatcv.o\
												 printfUtils/build/printfutl.o

all: $(OBJDIR) $(SUBDIRS) $(LIB_COLORUTILS_TARGET)\
													$(LIB_PRINTFUTILS_TARGET)\
													$(LIB_PRINTHEXTABLE_TARGET)

$(OBJDIR):
	@mkdir -p $@

$(SUBDIRS):
	$(MAKE) -C $@

# Archive final static libs
$(LIB_COLORUTILS_TARGET): $(LIB_COLORUTILS_OBJS)
	@printf "  AR\t%s\n" $@
	@$(AR) rcs $@ $^

$(LIB_PRINTFUTILS_TARGET): $(LIB_PRINTFUTILS_OBJS)
	@printf "  AR\t%s\n" $@
	@$(AR) rcs $@ $^

$(LIB_PRINTHEXTABLE_TARGET): $(LIB_PRINTHEXTABLE_OBJS)
	@printf "  AR\t%s\n" $@
	@$(AR) rcs $@ $^


clean:
	@for d in $(SUBDIRS); do $(MAKE) -C $$d clean; done
	@rm -rf $(OBJDIR)

