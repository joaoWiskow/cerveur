CC = gcc
CFLAGS = -Iinclude -pg -Wall
TARGET = server.out
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
LDFLAGS = -lm -ldl -fPIC -rdynamic -pg

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm -f *.out *.o *.a src/*.a src/*.o gmon.out relatorio.txt

profile: $(TARGET)
	@./$(TARGET) 6969
	@gprof $(TARGET) gmon.out > relatorio.txt
	@cat relatorio.txt
