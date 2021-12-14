/*

Dictionaries
Copyright 2014-2017, Ciaran Anscomb

This is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

*/

#include "config.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "c-strcase.h"
#include "hash.h"
#include "xalloc.h"

#include "dict.h"
#include "slist.h"

struct dict {
	Hash_hasher hash_func;
	Hash_comparator key_equal_func;
	Hash_data_freer key_destroy_func;
	Hash_data_freer value_destroy_func;
	Hash_table *ht;
};

struct dict_ent {
	struct dict *dict;
	void *key;
	void *value;
};

struct dict_foreach_data {
	dict_iter_func func;
	void *data;
};

static size_t dict_ent_hash(const void *, size_t);
static bool dict_ent_equal(const void *, const void *);
static void dict_ent_free(void *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* Create new dictionary. */

struct dict *dict_new(Hash_hasher hash_func,
		     Hash_comparator key_equal_func) {
	return dict_new_full(hash_func, key_equal_func, NULL, NULL);
}

struct dict *dict_new_full(Hash_hasher hash_func,
			  Hash_comparator key_equal_func,
			  Hash_data_freer key_destroy_func,
			  Hash_data_freer value_destroy_func) {
	struct dict *new = xmalloc(sizeof(*new));
	new->hash_func = hash_func;
	new->key_equal_func = key_equal_func;
	new->key_destroy_func = key_destroy_func;
	new->value_destroy_func = value_destroy_func;
	new->ht = hash_initialize(0, NULL, dict_ent_hash, dict_ent_equal, dict_ent_free);
	return new;
}

/* Clear all entries in dictionary and free its allocation. */

void dict_destroy(struct dict *d) {
	hash_free(d->ht);
	free(d);
}

/* Find an entry by key. */

static struct dict_ent *dict_find_ent(struct dict *d, const void *k) {
	const struct dict_ent q = {
		.key = (void *)k,
		.dict = d
	};
	struct dict_ent *ent = hash_lookup(d->ht, &q);
	if (!ent)
		return NULL;
	return ent;
}

/* Return the value for a given key, if it exists in the dictionary. */

void *dict_lookup(struct dict *d, const void *k) {
	struct dict_ent *ent = dict_find_ent(d, k);
	if (!ent)
		return NULL;
	return (void *)ent->value;
}

static void dict_add_ent(struct dict *d, void *k, void *v, bool replace_key) {
	struct dict_ent *ent = dict_find_ent(d, k);
	if (ent) {
		if (replace_key) {
			if (d->key_destroy_func && ent->key)
				d->key_destroy_func(ent->key);
			ent->key = k;
		} else {
			if (d->key_destroy_func && k)
				d->key_destroy_func(k);
		}
		if (d->value_destroy_func && ent->value)
			d->value_destroy_func(ent->value);
		ent->value = v;
		return;
	}
	struct dict_ent *new = xmalloc(sizeof(*new));
	new->key = k;
	new->value = v;
	new->dict = d;
	if (!hash_insert(d->ht, new))
		xalloc_die();
}

/* Insert an entry into the dictionary.  If an entry already exists with the
 * supplied key, the old value is freed (if possible) then overwritten. */

void dict_insert(struct dict *d, void *k, void *v) {
	dict_add_ent(d, k, v, false);
}

/* Insert an entry into the dictionary.  If an entry already exists with the
 * supplied key, the old key and value are freed then overwritten. */

void dict_replace(struct dict *d, void *k, void *v) {
	dict_add_ent(d, k, v, true);
}

/* Convenience function for when k == v. */

void dict_add(struct dict *d, void *k) {
	dict_add_ent(d, k, k, true);
}

/* Remove an entry from the dictionary, freeing its key and value if
 * destructors are defined.  Returns true if an entry was found and removed. */

bool dict_remove(struct dict *d, const void *k) {
	const struct dict_ent q = {
		.key = (void *)k,
		.dict = d
	};
	struct dict_ent *ent = hash_delete(d->ht, &q);
	if (!ent)
		return false;
	dict_ent_free(ent);
	return true;
}

/* Remove an entry from the dictionary, but do not free key or value.  Returns
 * true if an entry was found and removed. */

bool dict_steal(struct dict *d, const void *k) {
	const struct dict_ent q = {
		.key = (void *)k,
		.dict = d
	};
	struct dict_ent *ent = hash_delete(d->ht, &q);
	if (!ent)
		return false;
	free(ent);
	return true;
}

/* Shim callback to interface to hash_do_for_each(). */

static bool dict_foreach_processor(void *e, void *d) {
	struct dict_ent *ent = e;
	struct dict_foreach_data *fd = d;
	fd->func((void *)ent->key, (void *)ent->value, fd->data);
	return 1;
}

/* Iterate over all elements in the dictionary. */

void dict_foreach(struct dict *d, dict_iter_func func, void *data) {
	struct dict_foreach_data fd = {
		.func = func,
		.data = data
	};
	hash_do_for_each(d->ht, dict_foreach_processor, &fd);
}

static void add_key(void *k, void *v, struct slist **lp) {
	(void)v;  // unused
	*lp = slist_prepend(*lp, k);
}

static void add_value(void *k, void *v, struct slist **lp) {
	(void)k;  // unused
	*lp = slist_prepend(*lp, v);
}

struct slist *dict_get_keys(struct dict *d) {
	struct slist *l = NULL;
	dict_foreach(d, (dict_iter_func)add_key, &l);
	return l;
}

struct slist *dict_get_values(struct dict *d) {
	struct slist *l = NULL;
	dict_foreach(d, (dict_iter_func)add_value, &l);
	return l;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static size_t dict_ent_hash(const void *v, size_t tablesize) {
	const struct dict_ent *e = v;
	return e->dict->hash_func(e->key, tablesize);
}

static bool dict_ent_equal(const void *v1, const void *v2) {
	const struct dict_ent *e1 = v1;
	const struct dict_ent *e2 = v2;
	assert(e1->dict == e2->dict);
	return e1->dict->key_equal_func(e1->key, e2->key);
}

static void dict_ent_free(void *v) {
	struct dict_ent *e = v;
	if (e->dict->key_destroy_func && e->key)
		e->dict->key_destroy_func((void *)e->key);
	if (e->dict->value_destroy_func && e->value)
		e->dict->value_destroy_func((void *)e->value);
	free(e);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

size_t dict_direct_hash(const void *k, size_t tablesize) {
	return (size_t)k % tablesize;
}

bool dict_direct_equal(const void *k1, const void *k2) {
	return (k1 == k2);
}

bool dict_str_equal(const void *k1, const void *k2) {
	return (0 == strcmp((const char *)k1, (const char *)k2));
}

bool dict_str_equal_case(const void *k1, const void *k2) {
	return (0 == c_strcasecmp((const char *)k1, (const char *)k2));
}
