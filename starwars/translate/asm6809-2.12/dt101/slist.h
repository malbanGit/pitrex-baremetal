/*

Singly linked lists
Copyright 2009-2017, Ciaran Anscomb

This is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

A simple singly-linked list implementation.  The interfaces are similar
to other such implementations, but are implemented from scratch.  A list
is simply a pointer to its first member. In general, functions returning
a list are returning the new head of the list after an operation has
been performed.  Exceptions to this include copying.

*/

#ifndef DT101_SLIST_H__chqkIizgkKbjw
#define DT101_SLIST_H__chqkIizgkKbjw

/* Each list element is of this deliberately transparent type. */
struct slist {
	struct slist *next;
	void *data;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

/* Some operations want a user function as an argument.  Use these prototypes,
 * and cast accordingly. */

/* Used by iterators.  First arg is element data, second is user supplied. */
typedef void (*slist_iter_func)(void *, void *);

/* Used by slist_free_full() to free element data. */
typedef void (*slist_free_func)(void *);

/* Used by slist_copy_deep() to copy element data. */
typedef void *(*slist_copy_func)(const void *, void *);

/* Compare two structures, return <0, 0 or >0.  Used in sorting and finding. */
typedef int (*slist_cmp_func)(const void *, const void *);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

/* Each of these return the new pointer to the head of the list. */
struct slist *slist_append(struct slist *, void *);
struct slist *slist_prepend(struct slist *, void *);
struct slist *slist_insert_before(struct slist *, struct slist *, void *);
struct slist *slist_remove(struct slist *, void *);

/* Free entire list (not data). */
void slist_free(struct slist *);
/* Free entire list, calling function to free data. */
void slist_free_full(struct slist *, slist_free_func);
/* Free one list element. */
void slist_free_1(struct slist *);

/* Number of elements. */
unsigned slist_length(struct slist *);

/* Copy a list, new elements reference same data. */
struct slist *slist_copy(struct slist *);
/* Copy a list, new elements are copied with user function. */
struct slist *slist_copy_deep(struct slist *, slist_copy_func, void *);

/* Reverse a list. */
struct slist *slist_reverse(struct slist *);

/* Sort list in-place, the old list order is lost. */
struct slist *slist_sort(struct slist *, slist_cmp_func);

/* Attach head of one list to the end of another (no copying). */
struct slist *slist_concat(struct slist *, struct slist *);

/* Call a user function for each member of the list. */
void slist_foreach(struct slist *, slist_iter_func, void *);

/* Find element with exactly the supplied data (pointer compare). */
struct slist *slist_find(struct slist *, const void *);
/* Find element using compare function against supplied data. */
struct slist *slist_find_custom(struct slist *, const void *, slist_cmp_func);

#endif
