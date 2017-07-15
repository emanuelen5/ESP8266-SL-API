CC=gcc
CFLAGS=-Wall

OUTPUT=xml_parser
SRCS=$(OUTPUT).c
OBJS=$(SRCS:.c=.o)
.PHONY=all

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(OBJS)

$(OBJS): 
	$(CC) $(CFLAGS) -o $@ -c $(SRCS)

clean:
	rm -vf $(OBJS) $(OUTPUT)
