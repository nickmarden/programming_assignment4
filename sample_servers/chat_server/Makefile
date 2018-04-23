CC = gcc
CFLAGS = -O3 -Wall -Werror
CFLAGS += -fno-exceptions
CFLAGS += -finline-functions
CFLAGS += -funroll-loops

# Source files
SRC=chat.c chat_utils.c chat.h

chat_server: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm chat_server
    
