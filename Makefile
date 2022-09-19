test:
	gcc tests.c custom_malloc.c -o test.out -Wall -Wextra -Wshadow

run:
	./test.out

clean:
	rm test.out