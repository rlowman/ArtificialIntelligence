// File: execution_unit_list.c
// Author: Chad Hogg
// Version: 2014-10-21
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It contains definitions of functions for manipulating linked lists of
//   execution units.

#include <stdlib.h>

#include "execution_unit.h"
#include "execution_unit_list.h"

void enqueueExecutionUnit( struct ExecutionUnitList * list,
			   int mSecs,
			   int type ) {
  struct ExecutionUnitListNode * new_node = malloc( sizeof( struct ExecutionUnitListNode ) );
  new_node->next = NULL;
  new_node->data = malloc( sizeof( struct ExecutionUnit ) );
  new_node->data->mSecsLeft = mSecs;
  new_node->data->type = type;

  if( list->head == NULL ) {            // If it was empty, this is now the
    list->head = new_node;              //   first and last node.
    list->tail = new_node;
    new_node->prev = NULL;
  }
  else {                                // Otherwise add it to the end.
    new_node->prev = list->tail;
    list->tail->next = new_node;
    list->tail = new_node;
  }
}

void dequeueExecutionUnit( struct ExecutionUnitList * list ) {
  struct ExecutionUnitListNode * current = list->head;
  list->head = list->head->next;
  if( list->head != NULL ) {
    list->head->prev = NULL;
  } else {
    list->tail = NULL;
  }
  free( current->data );
  free( current );
}

