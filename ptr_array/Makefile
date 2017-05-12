CFLAGS += -Wall -Wextra -Werror -g

VALGRIND_OPTS = \
    --quiet --leak-check=full \
    --show-reachable=no --show-possibly-lost=no \
    --trace-children=yes --error-exitcode=1

test: ptr_array.o

default: test

check: check_mem_leak check_time

check_mem_leak: test
	valgrind $(VALGRIND_OPTS) ./test

check_time: test
	time -p ./test

clean:
	rm -f test vgcore* *.o
