CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic #$(shell pkg-config --cflags)

all: test banhammer

#test: bf.o bv.o city.o ht.o ll.o messages.o node.o parser.o
test: test.o bv.o node.o ll.o bf.o city.o ht.o parser.o
	$(CC) -o $@ $^

banhammer: banhammer.o bv.o node.o ll.o bf.o city.o ht.o parser.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	
spotless: clean
	rm -f test banhammer

format:
	clang-format -i -style=file *.[c]
