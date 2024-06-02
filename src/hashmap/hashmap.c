//
// Created by detjonmataj on 2024-05-17.
//

#include "hashmap.h"
#include "../utils.h"
#include <stdlib.h>
#include <string.h>

#define LOAD_FACTOR 0.75
#define INITIAL_SIZE 256

unsigned long hash(const char *str, const size_t size) {
    unsigned long hash = 5381;
    int c;

    // DJB2
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % size;
}

Hashmap *hashmap_init(void) {
    Hashmap *map = malloc(sizeof(Hashmap));
    if (map) {
        map->size = INITIAL_SIZE;
        map->buckets = (Entry **)calloc(map->size, sizeof(Entry *));
        map->count = 0;
    }
    return map;
}

void hashmap_resize(Hashmap *map, size_t new_size) {
    Entry **new_buckets = calloc(new_size, sizeof(Entry *));
    if (!new_buckets) {
        // TODO: Handle allocation failure
        return;
    }
    for (size_t i = 0; i < map->size; i++) {
        Entry *entry = map->buckets[i];
        while (entry != NULL) {
            Entry *next = entry->next;
            size_t index = hash(entry->key, new_size);
            entry->next = new_buckets[index];
            new_buckets[index] = entry;
            entry = next;
        }
    }
    free(map->buckets);
    map->buckets = new_buckets;
    map->size = new_size;
}

void hashmap_insert(Hashmap *map, const char *key, const char *value) {
    if (!map || !key || !value) {
        return;
    }
    if ((float)map->count / map->size >= LOAD_FACTOR) {
        hashmap_resize(map, map->size * 2);
    }
    const size_t index = hash(key, map->size);
    Entry *entry = malloc(sizeof(Entry));
    if (!entry) {
        return;
    }
    entry->key = cstringdup(key);
    entry->value = cstringdup(value);
    if (!entry->key || !entry->value) {
        free(entry->key);
        free(entry->value);
        free(entry);
        return;
    }
    entry->next = map->buckets[index];
    map->buckets[index] = entry;
    map->count++;
}

char *hashmap_get(Hashmap *map, const char *key) {
    if (!map || !key) {
        return NULL;
    }
    const size_t index = hash(key, map->size);
    const Entry *entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void hashmap_remove(Hashmap *map, const char *key) {
    if (!map || !key) {
        return;
    }
    const size_t index = hash(key, map->size);
    Entry *entry = map->buckets[index];
    Entry *prev = NULL;

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            if (prev == NULL) {
                map->buckets[index] = entry->next;
            } else {
                prev->next = entry->next;
            }
            free(entry->key);
            free(entry->value);
            free(entry);
            map->count--;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

int hashmap_contains_key(Hashmap *map, const char *key) {
    if (!map || !key) {
        return 0;
    }
    const size_t index = hash(key, map->size);
    const Entry *entry = map->buckets[index];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return 1;
        }
        entry = entry->next;
    }
    return 0;
}

size_t hashmap_size(Hashmap *map) {
    if (!map) {
        return 0;
    }
    return map->count;
}

void hashmap_clear(Hashmap *map) {
    if (!map) {
        return;
    }
    for (size_t i = 0; i < map->size; i++) {
        Entry *entry = map->buckets[i];
        while (entry != NULL) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
        map->buckets[i] = NULL;
    }
    map->count = 0;
}

void hashmap_free(Hashmap *map) {
    if (!map) {
        return;
    }
    for (size_t i = 0; i < map->size; i++) {
        Entry *entry = map->buckets[i];
        while (entry != NULL) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}

HashmapIterator *hashmap_iterator_init(Hashmap *map) {
    if (map == NULL) return NULL;

    HashmapIterator *iterator = malloc(sizeof(HashmapIterator));
    if (iterator == NULL) return NULL;

    iterator->map = map;
    iterator->index = 0;
    iterator->entry = NULL;

    return iterator;
}

Entry *hashmap_iterator_next(HashmapIterator *iterator) {
    if (iterator == NULL) return NULL;

    const Hashmap *map = iterator->map;
    if (map == NULL) return NULL;

    // If entry is present at current index and has a next entry (happens in case of key collision)
    if (iterator->entry != NULL) {
        iterator->entry = iterator->entry->next;
        if (iterator->entry != NULL) {
            return iterator->entry;
        }
    }

    // Find next non-empty bucket
    do {
        iterator->index++;
    } while (iterator->index < map->size && map->buckets[iterator->index] == NULL);

    // If a non-empty bucket is found
    if (iterator->index < map->size) {
        iterator->entry = map->buckets[iterator->index];
        return iterator->entry;
    }

    return NULL; // No more entries
}

void hashmap_iterator_free(HashmapIterator *iterator) {
    free(iterator);
}