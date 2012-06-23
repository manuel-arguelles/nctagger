/* 
 * Copyright (C) 2012  Manuel Argüelles <manuel.arguelles@gmail.com>
 *
 * This file is part of nctagger.
 *
 * Nctagger is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "list.h"

List *list_new(void)
{
    List *list = NULL;

    list = calloc(1, sizeof(List));

    return list;
}

List *list_add(List *list, void *data)
{
    if (list != NULL) {
        void *new_data = realloc(list->data, 
                                 ((list->len + 1) * sizeof(void *)));
        list->data = new_data;
        list->data[list->len] = data;
        list->len++;
    }
    return list;
}

void list_free(List *list, list_free_func free_func)
{
    if (list != NULL) {

        if (free_func != NULL) {
            int i;
            for (i = 0; i < list->len; i++)
                free_func(list->data[i]);
        }

        free(list->data);
        free(list);
    }
}

void *list_get(List *list, int pos)
{
    void *value = NULL;
    if (list != NULL && list->len > 0 && pos < list->len) {
        value = list->data[pos];
    }
    return value;
}

List *list_join(List *list1, List *list2)
{
    int i;
    if (list1 != NULL && list2 != NULL) {
        void *new_data = realloc(list1->data, 
                                 ((list1->len + list2->len) * sizeof(void *)));
        
        list1->data = new_data;
        for (i = 0; i < list2->len; i++)
            list1->data[list1->len + i] = list2->data[i];
        
        //list1->data[list1->len] = &list2->data;
        list1->len += list2->len;
    }

    return list1;
}

void list_foreach(List *list, void (*foreach_func)(void *))
{
    if (list != NULL && foreach_func != NULL) {
        int i;
        for (i = 0; i < list->len; i++)
            foreach_func(list->data[i]);
    }
}

List *list_sublist(List *list, int (*cmp_func)(void *))
{
    List *sublist = NULL;
    
    if (list != NULL && cmp_func != NULL) {
        int i;
        for (i = 0; i < list->len; i++)
            if (cmp_func(list->data[i]) == 1) {
                /* TODO: Add to sub list */
            }
    }

    return sublist;
}
