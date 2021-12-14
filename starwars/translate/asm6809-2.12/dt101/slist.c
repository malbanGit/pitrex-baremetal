/*

Singly linked lists
Copyright 2009-2017, Ciaran Anscomb

This is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

*/

#include "config.h"

#include <stdlib.h>

#include "slist.h"
#include "xalloc.h"

/* Wrap data in a new list container */
static struct slist *slist_new(void *data) {
	struct slist *new = malloc(sizeof(*new));
	if (!new) return NULL;
	new->next = NULL;
	new->data = data;
	return new;
}

/* Add new data to tail of list */
struct slist *slist_append(struct slist *list, void *data) {
	return slist_insert_before(list, NULL, data);
}

/* Add new data to head of list */
struct slist *slist_prepend(struct slist *list, void *data) {
	return slist_insert_before(list, list, data);
}

/* Insert new data before given position */
struct slist *slist_insert_before(struct slist *list, struct slist *before, void *data) {
	struct slist *ent = slist_new(data);
	struct slist *iter;
	if (!ent) return list;
	if (!list) return ent;
	ent->next = before;
	if (before == list) return ent;
	for (iter = list; iter; iter = iter->next) {
		if (!iter->next || iter->next == before) {
			iter->next = ent;
			break;
		}
	}
	return list;
}

/* Remove list entry containing data. */
struct slist *slist_remove(struct slist *list, void *data) {
	struct slist **entp;
	for (entp = &list; *entp; entp = &(*entp)->next) {
		if ((*entp)->data == data) break;
	}
	if (*entp) {
		struct slist *ent = *entp;
		*entp = ent->next;
		free(ent);
	}
	return list;
}

/* Free entire list (not data). */
void slist_free(struct slist *list) {
	slist_free_full(list, NULL);
}

/* Free entire list, calling function to free data. */
void slist_free_full(struct slist *list, slist_free_func free_func) {
	while (list) {
		struct slist *next = list->next;
		if (free_func && list->data)
			free_func(list->data);
		free(list);
		list = next;
	}
}

/* Free one list element. */
void slist_free_1(struct slist *list) {
	free(list);
}

/* Number of elements. */
unsigned slist_length(struct slist *list) {
	unsigned length = 0;
	while (list) {
		length++;
		list = list->next;
	}
	return length;
}

/* Copy a list, new elements reference same data. */
struct slist *slist_copy(struct slist *list) {
	struct slist *new = NULL;
	struct slist **entp = &new;
	for (; list; list = list->next) {
		*entp = slist_new(list->data);
		entp = &(*entp)->next;
	}
	return new;
}

/* Copy a list, new elements are copied with user function. */
struct slist *slist_copy_deep(struct slist *list, slist_copy_func copy_func, void *copy_data) {
	struct slist *new = slist_copy(list);
	for (struct slist *l = new; l; l = l->next) {
		l->data = copy_func(l->data, copy_data);
	}
	return new;
}

/* Reverse a list. */
struct slist *slist_reverse(struct slist *list) {
	struct slist *prev = NULL;
	while (list) {
		struct slist *next = list->next;
		list->next = prev;
		prev = list;
		list = next;
	}
	return prev;
}

/* Helper function for slist_sort().  Merges left and right into a single
 * sorted list, assuming inputs are sorted.  In-place. */
static struct slist *slist_merge(struct slist *left, struct slist *right, slist_cmp_func cmp_func) {
	struct slist *new = NULL;
	struct slist **newp = &new;
	while (1) {
		if (left && right) {
			if (cmp_func(left->data, right->data) <= 0) {
				*newp = left;
				left = left->next;
			} else {
				*newp = right;
				right = right->next;
			}
		} else if (left) {
			*newp = left;
			left = left->next;
		} else if (right) {
			*newp = right;
			right = right->next;
		} else {
			break;
		}
		newp = &(*newp)->next;
	}
	*newp = NULL;
	return new;
}

/* Sort list in-place, the old list order is lost. */

/* In-place merge sort.  This part breaks the list into trivially
 * sorted lists on the heap, then calls slist_merge() to merge the result. */
struct slist *slist_sort(struct slist *list, slist_cmp_func cmp_func) {
	if (!list || !list->next)
		return list;
	struct slist *left = NULL;
	struct slist **leftp = &left;
	struct slist *right = NULL;
	struct slist **rightp = &right;
	while (list) {
		*leftp = list;
		list = list->next;
		leftp = &(*leftp)->next;
		if (list) {
			*rightp = list;
			list = list->next;
			rightp = &(*rightp)->next;
		}
	}
	*leftp = *rightp = NULL;
	left = slist_sort(left, cmp_func);
	right = slist_sort(right, cmp_func);
	return slist_merge(left, right, cmp_func);
}

/* Attach head of one list to the end of another (no copying). */
struct slist *slist_concat(struct slist *list1, struct slist *list2) {
	if (!list2)
		return list1;
	if (!list1)
		return list2;

	struct slist *l = list1;
	while (l->next)
		l = l->next;
	l->next = list2;

	return list1;
}

/* Call a user function for each member of the list. */
void slist_foreach(struct slist *list, slist_iter_func iter_func, void *data) {
	struct slist *ent;
	for (ent = list; ent; ent = ent->next)
		iter_func(ent->data, data);
}

/* Find element with exactly the supplied data (pointer compare). */
struct slist *slist_find(struct slist *list, const void *data) {
	struct slist *ent;
	for (ent = list; ent; ent = ent->next) {
		if (ent->data == data)
			return ent;
	}
	return NULL;
}

/* Find element using compare function against supplied data. */
struct slist *slist_find_custom(struct slist *list, const void *data, slist_cmp_func cmp_func) {
	struct slist *ent;
	for (ent = list; ent; ent = ent->next) {
		if (cmp_func(ent->data, data) == 0)
			return ent;
	}
	return NULL;
}
