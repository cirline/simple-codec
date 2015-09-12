
target_file	:= simple-codec
test_file	:= test

CC		:= gcc
CFLAGS	:= -fPIC -L. -l$(target_file)

objs	:= base64.o

all: $(target_file) $(test_file)

$(target_file): $(objs)
	$(CC) -o lib$@.so -shared $^

$(test): test.o
	$(CC) $^ -o $@.out


clean:
	rm -rf *.out *.so *.o





