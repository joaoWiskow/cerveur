CC=gcc
CFLAGS=-Iinclude -pg -Wall
DEPS = HTTP_Server.h
TARGET = server.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic -I./include -pg

$(TARGET): $(objects)
	$(CC) $(objects) $(flags) -o $(TARGET)

%.o: %.c %.h
	$(CC) -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.a
	-rm src/*.o

profile: $(TARGET)
	./$(TARGET)
	gprof $(TARGET) gmon.out > relatorio.txt
	cat relatorio.txt
//colocar sleep para aparecer na tela