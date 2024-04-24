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

void enlarge(HashMap *map) {
  if (map == NULL) return;
  enlarge_called = 1; // no borrar (testing purposes)
  int i;
  Pair **aux = map->buckets;
  map->buckets = (Pair **)calloc(map->capacity * 2, sizeof(Pair *));
  map->capacity *= 2;
  for (i = 0; i < map->capacity / 2; i++) {
    if (aux[i] != NULL) {
      Pair *par = aux[i];
      while (par != NULL) {
        insertMap(map, par->key, par->value);
        par = par->next;
      }
    }
  }
}


void eraseMap(HashMap *map, char *key) {}

Pair *searchMap(HashMap *map, char *key) { return NULL; }

Pair *firstMap(HashMap *map) { return NULL; }

Pair *nextMap(HashMap *map) { return NULL; }
