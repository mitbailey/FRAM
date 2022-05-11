CC = gcc
RM = rm -vf
COBJS = example/main.o fram.o spibus/spibus.o
EDCFLAGS = -I ./ -I ./example/ -I ./spibus/ -Wall -pthread $(CFLAGS)
EDLDFLAGS := -lpthread -lm $(LDFLAGS)
TARGET = fram_test.out

all: $(COBJS)
	$(CC) $(EDCFLAGS) $(COBJS) -o $(TARGET) $(EDLDFLAGS)
	./$(TARGET)

%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean doc spotless

doc:
	doxygen .doxyconfig

clean:
	$(RM) *.out
	$(RM) $(COBJS)

spotless: clean
	$(RM) -r doc