# Compiler and archiver

CFLAGS = -Wall -Wextra -O2 -std=c99 -I. -I..
OBJDIR = build

# Sources and output
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))


#DEPS := ../build/libprintfutils.a ../build/libcolorutils.a

.PHONY: all clean

# Default target
all: $(OBJDIR) $(OBJS)

$(OBJDIR):
	@mkdir -p $@

# Compile source files
$(OBJDIR)/%.o: %.c
	@printf "  CC\t%s\n" $@
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)
