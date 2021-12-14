/*

Dictionaries
Copyright 2014-2017, Ciaran Anscomb

This is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

A dictionary implementation using an externally supplied hash table.
The hash table interface is expected to be as per Gnulib.

*/

#ifndef DT101_DICT_H__Kf6gSbxCcf8vw
#define DT101_DICT_H__Kf6gSbxCcf8vw

#include <stdbool.h>

#include "hash.h"
#include "hash-djb2.h"

struct dict;
struct slist;

typedef void (*dict_iter_func)(void *k, void *v, void *data);

struct dict *dict_new(Hash_hasher hash_func,
		     Hash_comparator key_equal_func);

struct dict *dict_new_full(Hash_hasher hash_func,
			  Hash_comparator key_equal_func,
			  Hash_data_freer key_destroy_func,
			  Hash_data_freer value_destroy_func);

void dict_destroy(struct dict *);

void *dict_lookup(struct dict *, const void *k);
void dict_insert(struct dict *, void *k, void *v);
void dict_replace(struct dict *d, void *k, void *v);
void dict_add(struct dict *d, void *k);
bool dict_remove(struct dict *, const void *k);
bool dict_steal(struct dict *, const void *k);
void dict_foreach(struct dict *, dict_iter_func, void *);
struct slist *dict_get_keys(struct dict *);
struct slist *dict_get_values(struct dict *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

size_t dict_direct_hash(const void *, size_t);
bool dict_direct_equal(const void *k1, const void *k2);

#define dict_str_hash (hash_djb2)
#define dict_str_hash_case (hash_djb2_case)
bool dict_str_equal(const void *k1, const void *k2);
bool dict_str_equal_case(const void *k1, const void *k2);

#endif
