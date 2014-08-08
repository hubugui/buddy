AR=ar
CC=gcc
DEL=rm -rf
MK=mkdir
OUT=objs
RM=rmdir /s /q
CFLAGS=-Wall -O3 -lm
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
TARGET=$(notdir $(CURDIR))
LIBS=lib$(TARGET).a

all: $(TARGET)

$(TARGET): $(LIBS)
	$(CC) -o $@ $^ $(CFLAGS) 
	-$(DEL) *.a

$(LIBS): $(OBJS)
	$(AR) -r $@ $^
	-$(DEL) *.o

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-$(DEL) *.a
	-$(DEL) *.o
	-$(DEL) *.out
	-$(DEL) *.lib

rebuild: clean all
