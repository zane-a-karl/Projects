#include "../hdr/str_hash_table.h"


struct StrHashTable *
new_str_hash_table ()
{
	struct StrHashTable *sht;
	sht = calloc(1, sizeof(struct StrHashTable));
	for (int i = 0; i < MAX_NUM_VARS; ++i) {
		sht->entries[i] = NULL;
	}
	return sht;
}

// Assume `name` is already calloc'd
struct StrHashEntry *
new_str_hash_entry (char *name)
{
	struct StrHashEntry *she;
	she = calloc(1, sizeof(struct StrHashEntry));
	she->name            = name;
	she->type            = DATA;
	she->data            = NULL;
	she->data_len        = 0;
	she->next            = NULL;

	return she;
}

unsigned int
sht_hash (char *name)
{
	int len = strnlen(name, MAX_VAR_NAME_LEN);
	unsigned int hash_value = 0;
	for (int i = 0; i < len; ++i) {
		hash_value += name[i];
		hash_value = (hash_value * name[i]) % MAX_VAR_NAME_LEN;
	}
	return hash_value;
}

bool
sht_insert (struct StrHashTable *sht,
						struct StrHashEntry *she)
{
	if ( she == NULL ) return false;
	int index = sht_hash(she->name);
	she->next = sht->entries[index];
	sht->entries[index] = she;

	return true;
}

struct StrHashEntry *
sht_lookup (struct StrHashTable *sht,
						char                *name)
{
	int index = sht_hash(name);
	struct StrHashEntry *i = sht->entries[index];
	while ( i != NULL &&
					strncmp(i->name, name, MAX_VAR_NAME_LEN) != 0 ) {

		i = i->next;
	}
	return i;
}

struct StrHashEntry *
sht_delete (struct StrHashTable *sht,
						char                *name)
{
  int index = sht_hash(name);
	struct StrHashEntry *i = sht->entries[index];
	struct StrHashEntry *prv = NULL;
	while ( i != NULL &&
					strncmp(i->name, name, MAX_VAR_NAME_LEN) != 0 ) {
		
		prv = i;
		i = i->next;
	}
  	if ( i == NULL) {       //empty list head
		return NULL;
	} else if ( prv == NULL) {//head only list
		sht->entries[index] = i->next;
	} else {                  //str found in list
		prv->next = i->next;
	}
	return i;
}

char *
deep_copy_str (char *src)
{
	char *dst = calloc(MAX_VAR_NAME_LEN, sizeof(char));
	for (int i = 0; i < MAX_VAR_NAME_LEN; ++i) {
		dst[i] = src[i];
	}
	return dst;
}

// Assume `dst` is already calloc'd
void
deep_copy_sht_entries (struct StrHashTable *src,
											 struct StrHashTable *dst)
{
	int i, k;
	struct StrHashEntry *j, *tmp;
	for (i = 0; i < MAX_NUM_VARS; ++i) {
		for (j = src->entries[i]; j != NULL; j = j->next) {
			tmp = new_str_hash_entry(deep_copy_str(j->name));
			sht_insert(dst, tmp);
			if (j->type == NODE) {
				tmp->node = j->node;
			} else if (j->type == DATA) {
				tmp->data = calloc(j->data_len, sizeof(int));
				for (k = 0; i < j->data_len; ++k) {
					tmp->data[k] = j->data[k];
				}
			} else {
				perror("Unrecognized she type");
				exit(1);
			}
		}
	}
}

// Remember if you used string literals then DO NOT free!
void
free_she (struct StrHashEntry **she)
{
	free((*she)->name);
	if ( (*she)->type == DATA ) {
		free((*she)->data);
	}
	free(*she);
}

void
free_sht (struct StrHashTable **sht)
{
	for (int i = 0; i < MAX_NUM_VARS; ++i) {
		struct StrHashEntry *tmp = (*sht)->entries[i];
		struct StrHashEntry *prv = NULL;
			while ( tmp != NULL ) {
				prv = tmp->next;
				free_she(&tmp);
				tmp = prv;
			}
	}
	free(*sht);
}

void
print_table (struct StrHashTable *sht)
{
	printf("START\n");
	for (int i = 0; i < MAX_NUM_VARS; ++i) {
		if ( sht->entries[i] == NULL ) {
			printf("\t%i\t---\n", i);
		} else {
			printf("\t%i\t", i);
			struct StrHashEntry *tmp = sht->entries[i];
			while ( tmp != NULL ) {
				printf("%s -> ", tmp->name);
				tmp = tmp->next;
			}
			printf("\n");
		}
	}
	printf("END\n");
}
