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

#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stdint.h>

#include "ptr_list.h"

struct _hash_table;

/*
 * Return NULL if no enough memory and errno will be set as ENOMEM.
 */
struct _hash_table *_hash_table_new(void);

/*
 * The memory hold by 'key' will be copied. But memory hold by 'value' will
 * __not__ be copied.
 * Both key and value should not be NULL.
 * Fail when no enough memory, ENOMEM will be returned.
 * Fail when identical entry already exists, EEXIST will be returned.
 */
int _hash_table_set(struct _hash_table *h, const char *key, void *value);

/*
 * If not found, NULL will be returned and errno will be set to ESRCH.
 */
void *_hash_table_get(struct _hash_table *h, const char *key);

/*
 * This function does not touch the memory used by 'value', make sure
 * you free it before invoke _hash_table_del().
 * Return 0 if success. Return ESRCH if not found.
 */
int _hash_table_del(struct _hash_table *h, const char *key);

/*
 * Only free internal memory(including copied key memory).
 * The memory hold by value will be untouched.
 * Use _hash_table_free_all() macro when you want to free value memory as well.
 */
void _hash_table_free(struct _hash_table *h);

struct _pointer_list *_hash_table_key_list_get(struct _hash_table *h);

struct _hash_table *_hash_table_copy(struct _hash_table *h);

/*
 * _hash_table_for_each will skip NULL value and NULL key.
 */
#define _hash_table_for_each(h, key_list, i, key, value) \
	for (i = 0; \
	     (h != NULL) && (key_list = _hash_table_key_list_get(h)) && \
	     (key = _ptr_list_index(key_list, i)) && \
	     ((value = _hash_table_get(h, key) || 1)); \
	     ++i)

#define _hash_table_free_all(h, key_list, i, key, value, value_free_func) \
	do { \
		_hash_table_for_each(h, key_list, i, key, value) \
			value_free_func(value); \
		_hash_table_free(h); \
	}while(0)

#endif  /* End of _HASH_TABLE_H_ */
