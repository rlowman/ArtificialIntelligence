// File: memory_access_list.c
// Author: Chad Hogg
// Version: 2014-11-09
// This file is part of project 03 in CS364 at King's College in Fall 2014.
// It defines functions for interacting with linked lists of MemoryAccess
//   structures.

#include <stdlib.h>
#include <stdbool.h>

#include "memory_access.h"
#include "memory_access_list.h"

void enqueueMA( struct MemoryAccessList * list,
		struct MemoryAccess * newItem ) {
  struct MemoryAccessListNode * newNode = malloc(sizeof(struct MemoryAccessListNode));
  newNode->data = newItem;
  if( list->tail == NULL ) {
    list->head = list->tail = newNode;
    newNode->next = NULL;
  }
  else {
    list->tail->next = newNode;
    list->tail = newNode;
  }
}

struct MemoryAccess * dequeueMA( struct MemoryAccessList * list ) {
  struct MemoryAccessListNode * temp = list->head;
  if( temp == NULL ) {
    return NULL;
  } else {
    struct MemoryAccess * value = temp->data;
    list->head = temp->next;
    if( list->head == NULL ) {
      list->tail = NULL;
    }
    free(temp);
    return value;
  }
}
