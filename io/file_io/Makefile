CC = gcc
CFLAGS = -g
SOURCE = $(wildcard *.c)
TARGET = $(notdir $(CURDIR))
OBJ    = $(patsubst %.c, %.o,$(SOURCE))
all : $(TARGET)
	@echo build $(TARGET)
$(TARGET):$(OBJ)
	@echo linking $@ $^
	$(CC) $(CFLAGS) -o $@ $^
$(OBJ): %.o:%.c 
	@echo compiling $@ form $<
	$(CC) $(CFLAGS) -c $< -o $@
	@echo finished
.PHONY : clean
clean:
	@echo remove files
	rm -rf $(TARGET) *.o


