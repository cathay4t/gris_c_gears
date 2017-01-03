TEST_EXEC = test_hash_table

CFLAGS = -Wall -Wextra -Werror -g -I.
OBJS = hash_table.o ptr_list.o $(TEST_EXEC).o

#hash_table.o: hash_table.c hash_table.h
#	gcc -c hash_table.c $(CFLAGS)

#ptr_list.o: ptr_list.c ptr_list.h
#	gcc -c ptr_list.c $(CFLAGS)

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
