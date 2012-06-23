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

#ifndef NCTAGGER_LIST
#define NCTAGGER_LIST

struct List_ {
    int len;
    void **data;
};

typedef struct List_ List;

/* Callback used when freeing the list */
typedef void (*list_free_func) (void *);

/* 
 * list_new creates and returns a newly and empty allocated List
 * object or null if it fails.
 */
List *list_new(void);

/*
 * list_add stores the pointer into the list object.
 */ 
List *list_add(List *list, void *data);

/*
 * list_free frees the memory used to store the elements (pointers),
 * if free_func is not null it will be called for each element.
 */
void list_free(List *list, list_free_func free_func);

/*
 * list_get returns the element (pointer) stored on the location pos.
 */
void *list_get(List *list, int pos);

/*
 * list_join merge two List objects, list2 is appended to list1
 */
List *list_join(List *list1, List *list2);

/*
 * list_foreach executes function foreach_func for for every element
 * in the list
 */
void list_foreach(List *list, void (*foreach_func)(void *));

#endif
