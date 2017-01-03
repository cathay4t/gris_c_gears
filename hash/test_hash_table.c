/*
 * Copyright (C) 2016 Gris Ge
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Gris Ge <cnfourt@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "hash_table.h"

const char * const keys[] = {"A", "B", "C", "D", "E"};
const char * const values[] = {"a", "b", "c", "d", "e"};

int main(void) {
	int rc = EXIT_FAILURE;
	struct _hash_table *h = NULL;
	struct _pointer_list *key_list = NULL;
	size_t i = 0;
	uint32_t j = 0;
	char *value = NULL;
	const char *key = NULL;
	int tmp_rc = 0;

	h = _hash_table_new();
	if (h == NULL)
		goto out;

	for (i = 0; i < sizeof(keys)/sizeof(keys[0]); ++i) {
		value = strdup(values[i]);
		if (value == NULL) {
			printf("No memory\n");
			goto out;
		}
		tmp_rc = _hash_table_set(h, keys[i], value);
		if (tmp_rc != 0) {
			printf("_hash_table_set() failed %d\n", tmp_rc);
			free(value);
			goto out;
		}
	}

	_hash_table_for_each(h, key_list, j, key, value)
		printf("key: '%s', value '%s'\n", key, value);

	value = _hash_table_get(h, "A");
	printf("deleting key A: '%s'\n", value);
	free(value);
	_hash_table_del(h, "A");
	value = _hash_table_get(h, "A");
	assert(value == NULL);
	_hash_table_for_each(h, key_list, j, key, value)
		printf("key: '%s', value '%s'\n", key, value);
	value = strdup("a");
	if (value == NULL) {
		printf("No memory\n");
		goto out;
	}
	printf("adding key A: '%s'\n", value);
	tmp_rc = _hash_table_set(h, "A", value);
	if (tmp_rc != 0) {
		printf("_hash_table_set() failed %d\n", tmp_rc);
		free(value);
		goto out;
	}
	_hash_table_for_each(h, key_list, j, key, value)
		printf("key: '%s', value '%s'\n", key, value);

	rc = EXIT_SUCCESS;
out:
	_hash_table_free_all(h, key_list, j, key, value, free);
	return rc;
}
