CC = C:/Develop/Tools/MSYS2/mingw32/bin/gcc.exe
CFLAGS = -g -Wall -std=gnu17

# Folders
OBJDIR = build
BINDIR = bin

# Timestamp for release builds (YYYYMMDD_HHMM)
TIMESTAMP := $(shell date +%Y%m%d_%H%M)

# Targets
DEBUG_TARGET = $(BINDIR)/main.exe
RELEASE_TARGET = $(BINDIR)/main_$(TIMESTAMP).exe

# Source and object files
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

# Default target
.DEFAULT_GOAL := debug

debug: $(DEBUG_TARGET)

release: $(RELEASE_TARGET)

$(DEBUG_TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(DEBUG_TARGET)

$(RELEASE_TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(RELEASE_TARGET)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)