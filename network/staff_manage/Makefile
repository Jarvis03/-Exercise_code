CC=gcc
CFLAGS = -g

SOURCE = $(wildcard *.c)
TARGET = $(notdir $(CURDIR))
#TARGET = DEMO
OBJ    = $(patsubst %.c, %.o, $(SOURCE))

#echo build $(TARGET)
all : $(TARGET)
	@echo build $(TARGET)
$(TARGET):$(OBJ)
	@echo linking $@ $^
	$(CC) -o $@ $^
$(OBJ): %.o:%.c
	@echo compiling $@ $^
	$(CC) -c $< -o $@
	@echo finished

.PHONY: clean
clean :
	@echo remove files
	rm -rf $(TARGET) *.o
