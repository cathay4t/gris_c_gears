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
#include <stdbool.h>

struct _hash_table;

/*
 * Return NULL if no enough memory and errno will be set as ENOMEM.
 * If 'value_free_func' is NULL, memory hold by value does not require manual
 * free.
 */
struct _hash_table *_hash_table_new(bool need_free_key,
				    void (*value_free_func)(void *value));

/*
 * The 'key' should not be NULL.
 * Fail when no enough memory, ENOMEM will be returned.
 * When identical entry already exists, old value will be freed by
 * `value_free_func()`, old key will be freed if `need_free_key` is true.
 */
int _hash_table_set(struct _hash_table *h, const char *key, void *value);

/*
 * If not found, NULL will be returned and errno will be set to ESRCH.
 */
void *_hash_table_get(struct _hash_table *h, const char *key);

/*
 * Memory used by key will be freed if `need_free_key` is true.
 * Memory used by value will be freed by `value_free_func()`.
 * Return 0 if success. Return ESRCH if not found.
 */
int _hash_table_del(struct _hash_table *h, const char *key);

void _hash_table_free(struct _hash_table *h);

#endif  /* End of _HASH_TABLE_H_ */
