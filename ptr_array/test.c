/*
 * Copyright (C) 2016 Red Hat, Inc.
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Gris Ge <fge@redhat.com>
 */
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "ptr_array.h"
#include "test_data.h"

int main(void) {
	const char *tmp_str = NULL;
	const char *test_string = NULL;
	struct ptr_array *pa = NULL;
	uint32_t i = 0;
	uint32_t test_string_size =
		sizeof(test_strings) / sizeof(test_strings[0]);

	pa = ptr_array_new(test_string_size);

	for (i = 0; i < test_string_size; ++i) {
		ptr_array_update(pa, i, (void *) test_strings[i]);
	}
	/* Add a dup into to test grow */
	for (i = 0; i < test_string_size; ++i) {
		ptr_array_insert(pa, (void *) test_strings[i]);
	}

	assert(ptr_array_size(pa) == test_string_size * 2);

	for (i = 0; i < test_string_size * 2; ++i) {
		tmp_str = ptr_array_get(pa, i);
		assert(tmp_str != NULL);
		if (i >= test_string_size)
			test_string = test_strings[i - test_string_size];
		else
			test_string = test_strings[i];
		assert(strcmp(tmp_str, test_string) == 0);
	}
	ptr_array_free(pa);
	printf("PASS with %" PRIu32 " strings\n", test_string_size);
	exit(EXIT_SUCCESS);
}
