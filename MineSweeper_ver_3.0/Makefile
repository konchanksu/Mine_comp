CC      = hgcc
HEADER  = Array.h Click.h Description.h
OBJECT	= MineSweeper_ver_3.0.c
TARGET  = a.out

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) -o $@ $(OBJECT)

$(OBJECT): $(HEADER)

test: $(TARGET)
	./$(TARGET)

