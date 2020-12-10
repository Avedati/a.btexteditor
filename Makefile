CC=gcc
CFLAGS=$(shell pkg-config gtk+-3.0 --cflags --libs)
TARGETS=src/*.c
OUT=bin/a.b_texteditor

all:
	$(CC) $(CFLAGS) $(TARGETS) -o $(OUT)

clean:
	rm -rf $(OUT)

test:
	$(MAKE) all
	./$(OUT)
	$(MAKE) clean
