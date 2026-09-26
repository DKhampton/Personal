CC = C:/Develop/Tools/MSYS2/mingw32/bin/gcc.exe
INCLUDE_SPECIAL := ./include/special/
INCLUDE_COMMON := ./include/
INCLUDE_SETUP := ./

CFLAGS = -g -Wall -std=gnu17 -I$(INCLUDE_SETUP) -I$(INCLUDE_COMMON) -I$(INCLUDE_SPECIAL)
LDFLAGS = -g

SRCDIR = sources
OBJDIR = build

TIMESTAMP := $(shell date +%Y%m%d_%H%M)
DEBUG_TARGET := ./main.exe
RELEASE_TARGET := ./main_$(TIMESTAMP).exe

# Recursively find every .c file under SRCDIR, no matter how deep
SRCS := $(shell find $(SRCDIR) -name '*.c')
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.DEFAULT_GOAL := debug

debug: $(DEBUG_TARGET)
release: $(RELEASE_TARGET)

$(DEBUG_TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(DEBUG_TARGET)

$(RELEASE_TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(RELEASE_TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(DEBUG_TARGET) $(RELEASE_TARGET)