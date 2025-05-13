# colorUtils/Makefile

# Variables
CFLAGS = -Wall -Wextra -O2 -std=c99 -I.
OBJDIR = build

# Sources and objects
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))


.PHONY: all clean

# Default target
all: $(OBJDIR) $(OBJS)

$(OBJDIR):
	@mkdir -p $@

# Compile source files to build/
$(OBJDIR)/%.o: %.c
	@printf "  CC\t%s\n" $@
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)
