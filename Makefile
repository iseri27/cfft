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

clean:
	$(RM) *.o *.exe $(TARGET)

uninstall: clean
