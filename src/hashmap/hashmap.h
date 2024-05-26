//
// Created by detjonmataj on 2024-05-17.
//

#ifndef HTTPSERVER_C_HASHMAP_H
#define HTTPSERVER_C_HASHMAP_H

#include <stddef.h>

typedef struct Entry {
    char *key;
    char *value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry **buckets;
    size_t size;
    size_t count;
} Hashmap;

typedef struct {
    Hashmap *map;
    size_t index;
    Entry *entry;
} HashmapIterator;

unsigned long hash(const char *str, size_t size);
Hashmap *hashmap_init(void);
void hashmap_resize(Hashmap *map, size_t new_size);
void hashmap_insert(Hashmap *map, const char *key, const char *value);
char *hashmap_get(Hashmap *map, const char *key);
void hashmap_remove(Hashmap *map, const char *key);
int hashmap_contains_key(Hashmap *map, const char *key);
size_t hashmap_size(Hashmap *map);
void hashmap_clear(Hashmap *map);
void hashmap_free(Hashmap *map);

HashmapIterator *hashmap_iterator_init(Hashmap *map);
Entry *hashmap_iterator_next(HashmapIterator *iterator);
void hashmap_iterator_free(HashmapIterator *iterator);

#endif // HTTPSERVER_C_HASHMAP_H
