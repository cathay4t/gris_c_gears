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

#define _GNU_SOURCE	/* For hsearch_r() */

#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>
#include <stdio.h> // only for printf
#include <stdbool.h>

#include "hash_table.h"
#include "ptr_list.h"

#define _INITAL_MAX_ENTRIE_COUNT	128

struct _hash_table {
	struct _pointer_list *key_list;
	struct _pointer_list *all_key_list;
	/* 'all_key_list' is holding all keys including deleted one */
	struct hsearch_data *htab;
	size_t max_cap;
};

/*
 * Increase the internal hash table's max capacity.
 */
static int _hash_table_grow(struct _hash_table *h);

static int _hash_table_grow(struct _hash_table *h)
{
	struct hsearch_data *new_htab = NULL;
	struct _pointer_list *key_list = NULL;
	const char *key = NULL;
	void *value = NULL;
	ENTRY *retval = NULL;
	uint32_t i = 0;
	ENTRY e;

	assert(h != NULL);
	assert(h->htab != NULL);
	assert(h->max_cap > 0);

	new_htab = (struct hsearch_data *) malloc(sizeof(struct hsearch_data));
	if (new_htab == NULL)
		goto nomem;

	memset(new_htab, 0, sizeof(struct hsearch_data));

	hcreate_r(h->max_cap * 2, new_htab);

	/* Copy key and values */
	_hash_table_for_each(h, key_list, i, key, value) {
		e.key = (char *) key;
		e.data = value;
		if (hsearch_r(e, ENTER, &retval, new_htab) != 0)
			goto nomem;
	}

	hdestroy_r(h->htab);
	h->htab = new_htab;
	errno = 0;
	return 0;

nomem:
	if (new_htab != NULL)
		hdestroy_r(new_htab);
	errno = ENOMEM;
	return errno;
}

struct _hash_table *_hash_table_new(void)
{
	struct _hash_table *h = NULL;

	h = (struct _hash_table *) malloc(sizeof(struct _hash_table));
	if (h == NULL)
		goto nomem;

	memset(h, 0, sizeof(struct _hash_table));
	h->key_list = _ptr_list_new();
	if (h->key_list == NULL)
		goto nomem;

	h->all_key_list = _ptr_list_new();
	if (h->all_key_list == NULL)
		goto nomem;

	h->max_cap = _INITAL_MAX_ENTRIE_COUNT;
	/* according to manpage, hcreate_r() only fail for htab is NULL
	 * which will never happen here. Hence we don't check return of
	 * hcreate_r() and assume it succeeded.
	 */
	h->htab = (struct hsearch_data *) malloc(sizeof(struct hsearch_data));
	if (h->htab == NULL)
		goto nomem;
	memset(h->htab, 0, sizeof(struct hsearch_data));
	hcreate_r(h->max_cap, h->htab);

	errno = 0;
	return h;

nomem:
	if (h != NULL) {
		_ptr_list_free(h->key_list);
		_ptr_list_free(h->all_key_list);
		if (h->htab != NULL)
			hdestroy_r(h->htab);
		free(h);
	}
	errno = ENOMEM;
	return NULL;
}

int _hash_table_set(struct _hash_table *h, const char *key, void *value)
{
	ENTRY e;
	ENTRY *retval = NULL;
	uint32_t i = 0;
	const char *cur_key = NULL;
	bool found = false;

	assert(h != NULL);
	assert(key != NULL);
	assert(value != NULL);

	/* Check whether specified key already exists */
	if (_hash_table_get(h, key) != NULL) {
		errno = EEXIST;
		return errno;
	}

	e.key = NULL;
	e.data = value;

	/* Check specified key is old deleted key */
	_ptr_list_for_each(h->all_key_list, i, cur_key) {
		if (strcmp(cur_key, key) == 0) {
			found = true;
			break;
		}
	}

	if (found == false) {
		/* Make sure hash table max entry count is 25% larger
		 * than used. */
		if ((_ptr_list_len(h->all_key_list) >=
		     (int) (h->max_cap * 0.75)) &&
		    (_hash_table_grow(h) != 0))
				goto nomem;

		e.key = strdup(key);
		if (e.key == NULL)
			goto nomem;
	} else {
		e.key = (char *) cur_key;
	}

	if (_ptr_list_add(h->key_list, e.key) != 0)
		goto nomem;

	if ((found == false) && (_ptr_list_add(h->all_key_list, e.key) != 0)) {
		_ptr_list_del(h->key_list, _ptr_list_len(h->key_list) - 1);
		goto nomem;
	}

	if (hsearch_r(e, ENTER, &retval, h->htab) == 0) {
		if (found == false)
			_ptr_list_del(h->all_key_list,
				      _ptr_list_len(h->all_key_list) - 1);
		_ptr_list_del(h->key_list, _ptr_list_len(h->key_list) - 1);
		goto nomem;
	}

	if ((found == true) && (retval != NULL))
		retval->data = value;

	_ptr_list_for_each(h->key_list, i, cur_key) {
		printf("cur_key: %s\n", cur_key);
		printf("cur_value: %s\n", (char *) _hash_table_get(h, cur_key));
	}

	errno = 0;
	return 0;

nomem:
	if (found == false)
		free((char *) e.key);
	errno = ENOMEM;
	return errno;
}

void *_hash_table_get(struct _hash_table *h, const char *key)
{
	ENTRY e;
	ENTRY *retval = NULL;

	assert(h != NULL);
	assert(key != NULL);

	e.key = (char *) key;
	e.data = NULL;

	if (hsearch_r(e, FIND, &retval, h->htab) == 0) {
		errno = ESRCH;
		return NULL;
	}
	if (retval != NULL)
		return retval->data;
	return NULL;
}

/*
 * We just update hash table value as NULL.
 */
int _hash_table_del(struct _hash_table *h, const char *key)
{
	uint32_t i = 0;
	const char *cur_key = NULL;
	ENTRY *retval = NULL;
	ENTRY e;

	assert(key != NULL);

	e.key = (char *) key;
	e.data = NULL;

	if (hsearch_r(e, FIND, &retval, h->htab) == 0) {
		errno = ESRCH;
		return errno;
	}
	if (retval != NULL)
		retval->data = NULL;

	_ptr_list_for_each(h->key_list, i, cur_key) {
		if (strcmp(cur_key, key) == 0) {
			_ptr_list_del(h->key_list, i);
			break;
		}
	}

	return 0;
}

void _hash_table_free(struct _hash_table *h)
{
	uint32_t i = 0;
	const char *key = NULL;

	if (h == NULL)
		return;
	_ptr_list_for_each(h->all_key_list, i, key)
		free((char *) key);
	_ptr_list_free(h->all_key_list);
	_ptr_list_free(h->key_list);
	hdestroy_r(h->htab);
	free(h->htab);
	free(h);
}

struct _pointer_list *_hash_table_key_list_get(struct _hash_table *h)
{
	assert(h != NULL);
	return h->key_list;
}

struct _hash_table *_hash_table_copy(struct _hash_table *h)
{
	const char *key = NULL;
	void *value = NULL;
	struct _hash_table *new_h = NULL;
	struct _pointer_list *key_list = NULL;
	uint32_t i = 0;

	assert(h != NULL);

	new_h = _hash_table_new();
	if (new_h == NULL)
		goto nomem;

	_hash_table_for_each(h, key_list, i, key, value)
		if (_hash_table_set(new_h, key, value) != 0)
			goto nomem;

	return new_h;

nomem:
	if (new_h != NULL)
		_hash_table_free(new_h);
	errno = ENOMEM;
	return NULL;
}
