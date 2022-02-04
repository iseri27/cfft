SOURCES := $(wildcard ./*.c)
HEADERS := $(wildcard ./*.h)
OBJECTS := $(patsubst ./%.c, %.o, $(SOURCES))
CC = gcc
CFLAGS += -std=c11 -lcurses -g -DDEBUG
TARGET = cfft

$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@

test: utils.o test.o cf.o
	$(CC) $^ $(CFLAGS) -o $@

%.o: %.c
	$(CC) -c $^ $(CFLAGS) -o $@

install: $(TARGET)
	cp -f cfft /usr/local/bin/cfft

clean:
	$(RM) *.o *.exe $(TARGET) cfft test

uninstall: clean
	$(RM) /usr/local/bin/cfft
