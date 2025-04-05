#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

static struct pd_ht_entry *prev = NULL;
int pd_ht_debug = 2;
int pd_ht_errno;

size_t keylength(struct pd_ht *table, void *key)
{
	if (table->keylength) {
		return table->keylength;
	} else {
		return strlen(key) + 1;
	}
}

/* TODO this hash function sucks.  Short keys will cluster around low
 * hash values. */
int defaulthash(void *key, int keylen)
{
	int result = 0;

	while (keylen--) {
		result += *((char *) key + keylen);
	}
	return result;
}

static int pd_ht_keymatch(size_t keylen, void *key1, void *key2)
{
	if (keylen) {
		return !memcmp(key1, key2, keylen);
	} else {
		return !strcmp(key1, key2);
	}
}

struct pd_ht_entry *findinlist(struct pd_ht_entry *list, void *key)
{
	size_t keylen;

	prev = NULL;
	if (list == NULL) {
		/* empty entry bucket, key must not exist */
		pd_ht_errno = PD_HT_BADKEY;
		return NULL;
	}
/*      assert(list->table != NULL); */
	keylen = list->table->keylength;
	while (list != NULL) {
		if (pd_ht_keymatch(keylen, list->key, key)) {
			return list;
		}
		prev = list;
		list = list->next;
	}

	pd_ht_errno = PD_HT_BADKEY;
	return NULL;
}

static int resizetable(struct pd_ht *table, int newmax)
{
	struct pd_ht_entry **new, **old, *ent, *next;
	int oldmax, i, bucket;

	old = table->buckets;
	oldmax = table->bucketmax;

	new = malloc(sizeof(void *) * (newmax + 1));
	if (new == NULL) {
		pd_ht_errno = PD_HT_NOMEM;
		return oldmax;
	}
	for (i = 0; i <= newmax; i++) {
		new[i] = NULL;
	}

	for (i = 0; i <= oldmax; i++) {
		ent = old[i];
		while (ent != NULL) {
			bucket = ent->hash & newmax;
			next = ent->next;
			ent->next = new[bucket];
			new[bucket] = ent;
			ent = next;
		}
	}
	free(old);
	table->buckets = new;
	table->bucketmax = newmax;
	return newmax;
}

int pd_ht_size(struct pd_ht *table)
{
	return table->entryct;
}

int pd_ht_init(struct pd_ht *table, int keylength,
	       int (*hashfunc) (void *key, int length))
{
	int i;

	table->keylength = keylength;
	table->bucketmax = 3;
	table->entryct = 0;
	table->hash = hashfunc ? hashfunc : defaulthash;
	table->buckets = malloc(sizeof(void *) * (table->bucketmax + 1));
	if (table->buckets == NULL) {
		pd_ht_errno = PD_HT_NOMEM;
		return 0;
	}
	for (i = 0; i <= table->bucketmax; i++) {
		table->buckets[i] = NULL;
	}

	return 1;
}

struct pd_ht_entry *pd_ht_createentry(struct pd_ht *table, void *key)
{
	struct pd_ht_entry *ne, *old;
	int bucket = 0;
	size_t keylen;
	unsigned int hash;

	pd_ht_errno = 0;

	keylen = keylength(table, key);
	hash = table->hash(key, keylen);
	bucket = hash & table->bucketmax;
	/* see if it already exists */
	old = findinlist(table->buckets[bucket], key);
	if (old != NULL) {
		pd_ht_errno = PD_HT_EXISTS;
		return NULL;
	}
	ne = malloc(sizeof(struct pd_ht_entry));
	if (ne == NULL) {
		pd_ht_errno = PD_HT_NOMEM;
		return NULL;
	}
	ne->table = table;
	ne->hash = hash;

	/* TODO avoid a malloc if the key will fit into the pointer */
	ne->key = malloc(keylen);
	if (ne->key == NULL) {
		free(ne);
		pd_ht_errno = PD_HT_NOMEM;
		return NULL;
	}
	memcpy(ne->key, key, keylen);

	ne->data = NULL;
	ne->next = table->buckets[bucket];
	table->buckets[bucket] = ne;
	table->entryct++;

	if (table->entryct > table->bucketmax + 1) {
		table->bucketmax = resizetable(table, table->bucketmax * 2 + 1);
	}
	return ne;
}

struct pd_ht_entry *pd_ht_findentry(struct pd_ht *table, void *key)
{
	struct pd_ht_entry *entry;
	int bucket;
	size_t keylen;

	keylen = keylength(table, key);
	bucket = table->hash(key, keylen) & table->bucketmax;
	entry = findinlist(table->buckets[bucket], key);

	return entry;
}

int pd_ht_exists(struct pd_ht *table, void *key)
{
	if (pd_ht_findentry(table, key) == NULL) {
		return 0;
	} else {
		return 1;
	}
}

int pd_ht_deleteentry(struct pd_ht_entry *entry)
{
	unsigned int bucket;

	if (!pd_ht_exists(entry->table, entry->key)) {
		/* this should't happen, if it does it indicates that
		 * the ht memory is corrupt */
		pd_ht_errno = PD_HT_FATAL;
		return 0;
	}
	/* pd_ht_exists has the side effect of setting prev */
	bucket = entry->hash & entry->table->bucketmax;

	if (prev) {
		prev->next = entry->next;
	} else if (entry->table->buckets[bucket] == entry) {
		entry->table->buckets[bucket] = entry->next;
	} else {
		/* IMPOSSIBLE CONDITION */
		DEBUGHT(1, "Impossible condition in hashtable code.%s\n", "");
		abort();
	}
	entry->table->entryct--;

	/* TODO wont need to free this if we store the key in the pointer */
	free(entry->key);
	free(entry);

	return 1;
}

void *pd_ht_getvalue(struct pd_ht_entry *entry)
{
	return entry->data;
}

int pd_ht_setvalue(struct pd_ht_entry *entry, void *value)
{
	pd_ht_errno = 0;
	entry->data = value;
	return 1;
}

void *pd_ht_getkey(struct pd_ht_entry *entry)
{
	return entry->key;
}

void *pd_ht_fetch(struct pd_ht *ht, void *key)
{
	struct pd_ht_entry *e;

	pd_ht_errno = 0;

	e = pd_ht_findentry(ht, key);
	if (e == NULL) {
		pd_ht_errno = PD_HT_BADKEY;
		return NULL;
	}
	return pd_ht_getvalue(e);
}

/* error if bad key */
int pd_ht_store(struct pd_ht *ht, void *key, void *value,
		void (*disp) (void *old))
{
	struct pd_ht_entry *e;

	pd_ht_errno = 0;

	e = pd_ht_findentry(ht, key);
	if (e == NULL) {
		pd_ht_errno = PD_HT_BADKEY;
		return 0;
	}
	if (disp) {
		disp(e->data);
	}
	pd_ht_setvalue(e, value);
	return 1;
}

/* create key if bad, store otherwise */
int pd_ht_stash(struct pd_ht *ht, void *key, void *value,
		void (*disp) (void *old))
{
	struct pd_ht_entry *e;

	pd_ht_errno = 0;

	if (pd_ht_store(ht, key, value, disp)) {
		return 1;
	}
	/* oops, create key */
	/* TODO: we just looked for the key above, if we are here, we
	 * know the key doesn't exist, but createentry will check again */
	e = pd_ht_createentry(ht, key);
	if (e == NULL) {
		perror("malloc failure");
		return 0;
	}
	pd_ht_setvalue(e, value);
	return 2;
}

int pd_ht_keydelete(struct pd_ht *ht, void *key)
{
	struct pd_ht_entry *e;

	e = pd_ht_findentry(ht, key);
	/* TODO failure check */
	return pd_ht_deleteentry(e);
}

int pd_ht_dup(struct pd_ht *dest, struct pd_ht *src)
{
	int i;
	struct pd_ht_entry *o;

	pd_ht_init(dest, src->keylength, src->hash);
	/* pre-extend the bucket array */
	dest->bucketmax = resizetable(dest, src->bucketmax);

	for (i = 0; i <= dest->bucketmax; i++) {
		for (o = src->buckets[i]; o; o = o->next) {
			pd_ht_stash(dest, o->key, o->data, NULL);
			/* TODO check failure and don't use stash */
		}
	}

	return 1;
}

struct pd_ht_entry *pd_ht_firstentry(struct pd_ht *table,
				     struct pd_ht_search *search)
{

	search->table = table;
	search->bucket = 0;
	search->entry = table->buckets[0];
	while (search->entry == NULL && search->bucket <= search->table->bucketmax) {
		search->entry = search->table->buckets[++search->bucket];
	}

	return search->entry;
}

struct pd_ht_entry *pd_ht_nextentry(struct pd_ht_search *search)
{

	if (search->entry == NULL) {
		return NULL;
	}
	search->entry = search->entry->next;

	while (search->entry == NULL && search->bucket <= search->table->bucketmax) {
		search->entry = search->table->buckets[++search->bucket];
	}
	return search->entry;
}
