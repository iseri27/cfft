SOURCES := $(wildcard ./*.c)
HEADERS := $(wildcard ./*.h)
OBJECTS := $(patsubst ./%.c, %.o, $(SOURCES))

CC = gcc

NCURSES_CFLAGS = `pkg-config --cflags ncursesw`
NCURSES_LIBS =  `pkg-config --libs ncursesw`
CFLAGS += -std=c11 -g -DDEBUG $(NCURSES_CFLAGS)
TARGET = cfft

$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@ $(NCURSES_LIBS)

%.o: %.c
	$(CC) -c $^ $(CFLAGS) -o $@ $(NCURSES_LIBS)

install: $(TARGET)
	cp -f cfft /usr/local/bin/cfft

clean:
	$(RM) *.o *.exe $(TARGET) cfft test

uninstall: clean
	$(RM) /usr/local/bin/cfft
