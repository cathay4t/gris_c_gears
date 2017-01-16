/*
 * Copyright (C) 2016 Gris Ge
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
 * Author: Gris Ge <cnfourt@gmail.com>
 */

#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>
#include <stdio.h> // only for printf
#include <stdbool.h>

#include "hash_table.h"
#include "uthash.h"

#define _INITAL_MAX_ENTRIE_COUNT	128

#define _FREE_KEY(k) \
	if (h->need_free_key == true) \
		free(k);

#define _FREE_VALUE(v) \
	if (h->value_free_func != NULL) \
		h->value_free_func(v);

struct __entry {
	char *key;
	void *value;
	UT_hash_handle hh;
};

struct _hash_table {
	struct __entry *head;
	bool need_free_key;
	void (*value_free_func)(void *value);
};

struct _hash_table *_hash_table_new(bool need_free_key,
				    void (*value_free_func)(void *value))
{
	struct _hash_table *h = NULL;

	h = (struct _hash_table *) malloc(sizeof(struct _hash_table));
	if (h == NULL)
		goto nomem;

	memset(h, 0, sizeof(struct _hash_table));
	h->head = NULL;
	h->need_free_key = need_free_key;
	h->value_free_func = value_free_func;

	errno = 0;
	return h;

nomem:
	if (h != NULL)
		free(h);
	errno = ENOMEM;
	return NULL;
}

int _hash_table_set(struct _hash_table *h, const char *key, void *value)
{
	struct __entry *e = NULL;

	assert(h != NULL);
	assert(key != NULL);

	e = (struct __entry *) malloc(sizeof(struct __entry));
	if (e == NULL)
		goto nomem;

	e->key = (char *) key;
	e->value = value;

	_hash_table_del(h, key); /* ignore the error */

	HASH_ADD_STR(h->head, key, e);

	errno = 0;
	return 0;

nomem:
	if (e != NULL)
		free(e);
	errno = ENOMEM;
	return errno;
}

void *_hash_table_get(struct _hash_table *h, const char *key)
{
	struct __entry *e = NULL;

	assert(h != NULL);
	assert(key != NULL);

	HASH_FIND_STR(h->head, key, e);
	if (e == NULL)
		goto notfound;

	return e->value;

notfound:
	errno = ESRCH;
	return NULL;
}

int _hash_table_del(struct _hash_table *h, const char *key)
{
	struct __entry *e = NULL;

	assert(h != NULL);
	assert(key != NULL);

	HASH_FIND_STR(h->head, key, e);
	if (e == NULL)
		goto notfound;

	HASH_DEL(h->head, e);
	_FREE_KEY(e->key);
	_FREE_VALUE(e->value);
	free(e);

	errno = 0;
	return errno;

notfound:
	errno = ESRCH;
	return errno;
}

void _hash_table_free(struct _hash_table *h)
{
	struct __entry *e = NULL;
	struct __entry *tmp = NULL;

	if (h == NULL)
		return;

	HASH_ITER(hh, h->head, e, tmp) {
		HASH_DEL(h->head, e);
		_FREE_KEY(e->key);
		_FREE_VALUE(e->value);
		free(e);
	}

	free(h);
}
