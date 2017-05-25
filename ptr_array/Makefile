TEST_EXEC = ptr_array_test

CFLAGS = -Wall -Wextra -Werror -g -I.
OBJS = ptr_array.o $(TEST_EXEC).o

TEST_EXEC: $(OBJS)
	$(CC) -o $(TEST_EXEC) $(OBJS) $(CFLAGS)
	./$(TEST_EXEC)

check:
	valgrind --quiet --leak-check=full \
		--show-reachable=no --show-possibly-lost=no \
		--trace-children=yes --error-exitcode=1 \
		./$(TEST_EXEC)

clean:
	rm -fv $(OBJS) $(TEST_EXEC)
