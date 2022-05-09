CC = gcc
COBJS = src/main.o src/fram.o spibus/spibus.o
EDCFLAGS = -I ./ -I ./include/ -I ./spibus/ -Wall -pthread $(CFLAGS)
EDLDFLAGS := -lpthread -lm $(LDFLAGS)
TARGET = fram.out

all: $(COBJS)
	$(CC) $(EDCFLAGS) $(COBJS) -o $(TARGET) $(EDLDFLAGS)
	./$(TARGET)

%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	$(RM) *.out
	$(RM) *.o
	$(RM) src/*.o

.PHONY: spotless

spotless:
	$(RM) spibus/*.o