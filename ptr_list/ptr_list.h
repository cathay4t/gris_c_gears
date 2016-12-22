/*
 * Copyright (C) 2016 Red Hat, Inc.
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
 * Author: Gris Ge <fge@redhat.com>
 */

#ifndef _LIBNVME_PTR_LIST_H_
#define _LIBNVME_PTR_LIST_H_

#include <stdint.h>

struct _pointer_list;

/*
 * Return NULL if no memory
 */
struct _pointer_list *_ptr_list_new(void);

/*
 * Return -1 if no memory
 */
int _ptr_list_add(struct _pointer_list *ptr_list, void *data);

uint32_t _ptr_list_len(struct _pointer_list *ptr_list);

void *_ptr_list_index(struct _pointer_list *ptr_list, uint32_t index);

void _ptr_list_free(struct _pointer_list *ptr_list);

/*
 * Return -1 if no memory
 */
int _ptr_list_2_array(struct _pointer_list *ptr_list, void ***array,
		      uint32_t *count);

#define _ptr_list_for_each(l, i, d) \
	for (i = 0; \
	     (l != NULL) && (i < ptr_list_len(l)) && \
	     (d = ptr_list_index(l, i)); \
	     ++i)

#endif  /* End of _LIBNVME_PTR_LIST_H_  */
