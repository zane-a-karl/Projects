#ifndef _STR_HASH_TABLE_H_
#define _STR_HASH_TABLE_H_

#include "../hdr/constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum StrHashEntryType {DATA, NODE};

struct StrHashEntry {
	char                 *name;
	enum StrHashEntryType type;
	union {
		int                *data;
		struct AstNode     *node;
	};
	int                   data_len;
	struct StrHashEntry  *next;
};

struct StrHashTable {
	struct StrHashEntry *entries[MAX_NUM_VARS];
};

struct StrHashTable *
new_str_hash_table ();

struct StrHashEntry *
new_str_hash_entry (char *name);

unsigned int
sht_hash (char *name);

bool
sht_insert (struct StrHashTable *sht,
						struct StrHashEntry *she);

struct StrHashEntry *
sht_lookup (struct StrHashTable *sht,
						char                *name);

struct StrHashEntry *
sht_delete (struct StrHashTable *sht,
						char                *name);

char *
deep_copy_str (char *src);

void
deep_copy_sht_entries (struct StrHashTable *src,
											 struct StrHashTable *dst);

void
free_she (struct StrHashEntry **she);

void
free_sht (struct StrHashTable **sht);

void
print_table (struct StrHashTable *sht);

#endif//_STR_HASH_TABLE_H_
