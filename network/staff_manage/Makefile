CC=gcc
CFLAGS = -g

SOURCE = $(wildcard *.c)
#TARGET = $(notdir $(CURDIR))
TARGET = server client
OBJ    = $(patsubst %.c, %.o, $(SOURCE))

#echo build $(TARGET)
all : $(TARGET)
	
server:server.o
	@echo server linking $@ $^
	$(CC) -o $@ $^
client:client.o
	@echo client linking $@ $^
	$(CC) -o $@ $^
$(OBJ): %.o:%.c
	@echo compiling $@ $^
	$(CC) -c $< -o $@
	@echo finished

.PHONY: clean
clean :
	@echo remove files
	rm -rf $(TARGET) *.o
