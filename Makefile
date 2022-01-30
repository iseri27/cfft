SOURCES := $(wildcard ./*.c)
OBJECTS := $(patsubst ./%.c, %.o, $(SOURCES))
CC = gcc
CFLAGS += -std=c11 -g -DDEBUG
TARGET = cfft

$(TARGET): $(OBJECTS)
	$(CC) $^ $(CFLAGS) -o $@

%.o: %.c
	$(CC) -c $^ $(CFLAGS) -o $@

install: $(TARGET)
	cp -f cfft /usr/local/bin/cfft

clean:
	$(RM) *.o *.exe $(TARGET)

uninstall: clean
	$(RM) /usr/local/bin/cfft
