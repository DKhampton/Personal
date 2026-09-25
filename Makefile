CC = C:/Develop/Tools/MSYS2/mingw32/bin/gcc.exe
CFLAGS = -g -Wall -std=gnu17
TARGET = ad.exe
SRCS = \
	ADnDH/main.c \
	ADnDH/character.c \
	ADnDH/charutils.c \
	ADnDH/classes.c \
	ADnDH/consoles.c \
	ADnDH/files.c \
	ADnDH/global.c \
	ADnDH/sprintb.c \
	ADnDH/nodes.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)	