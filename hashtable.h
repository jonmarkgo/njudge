/* public domain by nathan wagner */
/* last modified Tue Sep 30 22:04:47 GMT 1997 */

#ifndef _PD_HT
#define _PD_HT

struct pd_ht;			/* predeclare it so we can have pointers to it */

struct pd_ht_entry {
	struct pd_ht_entry *next;	/* the next one in the bucket chain */
	struct pd_ht_entry *prev;
	struct pd_ht *table;
	int hash;
	void *key;
	void *data;
};

struct pd_ht {
	struct pd_ht_entry **buckets;
	int keylength;
	unsigned int bucketmax;
	int entryct;
	int (*hash) (void *key, int length);
};

struct pd_ht_search {
	struct pd_ht *table;
	unsigned int bucket;
	struct pd_ht_entry *entry;
};

int pd_ht_init(struct pd_ht *table, int keylength,
	       int (*hashfunc) (void *key, int length));
int pd_ht_delete(struct pd_ht *table);
int pd_ht_size(struct pd_ht *table);

struct pd_ht_entry *pd_ht_createentry(struct pd_ht *table, void *key);
int pd_ht_deleteentry(struct pd_ht_entry *entry);
struct pd_ht_entry *pd_ht_findentry(struct pd_ht *table, void *key);
void *pd_ht_getvalue(struct pd_ht_entry *entry);
int pd_ht_setvalue(struct pd_ht_entry *entry, void *value);
void *pd_ht_getkey(struct pd_ht_entry *entry);

struct pd_ht_entry *pd_ht_firstentry(struct pd_ht *table,
				     struct pd_ht_search *search);
struct pd_ht_entry *pd_ht_nextentry(struct pd_ht_search *search);
int pd_ht_exists(struct pd_ht *table, void *key);

void *pd_ht_fetch(struct pd_ht *ht, void *key);
int pd_ht_store(struct pd_ht *ht, void *key, void *value,
		void (*disp) (void *old));
int pd_ht_stash(struct pd_ht *ht, void *key, void *value,
		void (*disp) (void *old));
int pd_ht_keydelete(struct pd_ht *ky, void *key);

int pd_ht_dup(struct pd_ht *dest, struct pd_ht *src);

#define PD_HT_STRINGKEYS 0
#define PD_HT_FATAL 1
#define PD_HT_BADKEY 2
#define PD_HT_NOMEM 3
#define PD_HT_EXISTS 4

extern int pd_ht_debug;
extern int pd_ht_errno;

#ifndef NDEBUG
#define DEBUGHT(l,s,v) if (pd_ht_debug >= (l)) { fprintf(stderr, (s), (v));fflush(stderr);}
#else
#define DEBUGHT(l,s,v)
#endif

#endif				/* _PD_HT */
