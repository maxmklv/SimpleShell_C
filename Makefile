SRCS = main.c utils.c
BIN = main
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu11
LDFLAGS = -g
OBJS = $(SRCS:.c=.o)

all: $(BIN)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)  

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)  

clean:
	-rm -f $(OBJS)
	-rm -f $(BIN)
