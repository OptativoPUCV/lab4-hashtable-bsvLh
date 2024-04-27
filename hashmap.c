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
  if (map == NULL)
    return;
  enlarge_called = 1; // no borrar (testing purposes)
  Pair **buckets = map->buckets;
  long capacity = map->capacity;
  map->capacity *= 2;
  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
  map->size = 0;
  for (long i = 0; i < capacity; i++) {
    if (buckets[i] != NULL && buckets[i]->key != NULL) {
      insertMap(map, buckets[i]->key, buckets[i]->value);
    }
  }
  free(buckets);
}

void eraseMap(HashMap *map, char *key) 
{
  if (map == NULL || map->size == 0)
  {
    return;
  }
  long posicion = hash(key, map->capacity);
  while (map->buckets[posicion] != NULL)
    {
      if (is_equal(map->buckets[posicion]->key, key))
      {
        map->buckets[posicion]->key = NULL;
        map->size--;
        return;
      }
      posicion = (posicion + 1) % map->capacity;
    }
  
}

Pair *searchMap(HashMap *map, char *key) 
{  
  if (map == NULL) return NULL;
  long posicion = hash(key, map->capacity);
  while (map->buckets[posicion] != NULL)
    {
      if (is_equal(map->buckets[posicion]->key, key))
      {
        map->current = posicion;
        return map->buckets[posicion];
      }
      posicion = (posicion + 1) % map->capacity;
    }
  return NULL;
}

Pair *firstMap(HashMap *map)
{ 
  if (map == NULL || map->size == 0) return NULL;
  map->current = 0;
  while (map->buckets[map->current] == NULL || map->buckets[map->current]->key == NULL)
    {
      map->current = (map->current + 1) % map->capacity;
      if (map->current == 0) return NULL;
      
    }
  return map->buckets[map->current];
}
/*
Pair * nextMap(HashMap * map) 
{
  long pos = map->current;
  long aux = pos;
  do {
    pos = (pos + 1) % map->capacity;
    if(aux > pos) {
      return NULL;
    }
  } while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL);
  map->current = pos;
  return map->buckets[pos];
}
*/
Pair *nextMap(HashMap *map)
{
  if (map == NULL) return NULL;
  long aux = map->current++;
  
  while (map->buckets[aux] == NULL || map->buckets[aux]->key == NULL)
    {
      if (aux == map->capacity)
        return NULL;
      aux++;
    }
  map->current = aux;
  return map->buckets[map->current];

}
