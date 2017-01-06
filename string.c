/*
 * Copyright (C) 2015 Red Hat, Inc.
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

#define _msg_clear(x) x[0] = '\0'
#define _msg_set(x, len, format, ...) \
    { \
        snprintf(x, len, format, ##__VA_ARGS__); \
    }

static char *_trim_tailling_space(char *str);

static char *_trim_tailling_space(char *str) {
    char *end = NULL;
    if ((str == NULL) || (*str == '\0'))
        return;
    end = str + strlen(str) - 1;
    while(end > str && *end == ' ') --end;
    *(end + 1) = 0;
    return str;
}
