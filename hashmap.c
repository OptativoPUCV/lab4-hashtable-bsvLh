#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

HashMap *createMap(long capacity) {
  HashMap *mapa = (HashMap *)malloc(sizeof(HashMap));
  mapa->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
}

void insertMap(HashMap *map, char *key, void *value)
{
  if (map == NULL) return;
  if (map->size == map->capacity) enlarge(map);
  long posicion = hash(key, map->capacity);
  while (map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
    {
      if (is_equal(map->buckets[posicion]->key, key))
      {
        map->buckets[posicion]->value = value;
        return;
      }
      posicion = (posicion + 1) % map->capacity;
      
    }
  map->buckets[posicion] = createPair(key, value);
  map->size++;
  
}

void enlarge(HashMap *map) 
{
  enlarge_called = 1; // no borrar (testing purposes)
  if (map == NULL) return;
  Pair **old_buckets = map->buckets;
  long old_capacity = map->capacity;
  map->capacity = map->capacity * 2 + 1;
  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
  map->size = 0;
  long i = 0;
  while (i < old_capacity)
    {
      if (old_buckets[i] != NULL && old_buckets[i]->key != NULL)
      {
        insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
      }
      i++;
    }
  free(old_buckets);
}


void eraseMap(HashMap *map, char *key) {}

Pair *searchMap(HashMap *map, char *key) { return NULL; }

Pair *firstMap(HashMap *map) { return NULL; }

Pair *nextMap(HashMap *map) { return NULL; }
