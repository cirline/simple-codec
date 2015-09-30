
target_file	:= simple-codec
test_file	:= test

CC		:= gcc
CFLAGS	:= -fPIC -L. -l$(target_file)

objs	:= base64.o

all: $(target_file) $(test_file)
	make install

$(target_file): $(objs)
	$(CC) -o lib$@.so -shared $^

$(test_file): test.o
	$(CC) -o $@.out $^ $(CFLAGS)

install:
	sudo cp lib$(target_file).so /lib/


clean:
	rm -rf *.out *.so *.o





