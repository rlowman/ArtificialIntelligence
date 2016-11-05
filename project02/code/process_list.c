// File: process_list.c
// Author: Chad Hogg
// Version: 2014-10-21
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It contains definitions of functions for manipulating linked lists of
//   processes.

#include <stdlib.h>

#include "execution_unit.h"
#include "execution_unit_list.h"
#include "process.h"
#include "process_list.h"

void enqueueProcess( struct ProcessList * list,
		     struct Process * proc )
{
                                        // Create the new node.
  struct ProcessListNode * newNode = malloc( sizeof( struct ProcessListNode ) );
  newNode->next = NULL;
  newNode->data = proc;

  if( list->head == NULL ) {            // If it was empty this is now the
    list->head = newNode;               //   first and last node.
    list->tail = newNode;
    newNode->prev = NULL;
  }
  else {                                // Otherwise add it to the end.
    newNode->prev = list->tail;
    list->tail->next = newNode;
    list->tail = newNode;
  }
}

struct Process * dequeueProcess( struct ProcessList * list )
{
  struct Process * current = list->head->data;
  struct ProcessListNode * dying = list->head;
  if( list->head == list->tail ) {      // If this was the only node, then
    list->head = NULL;                  //   there are none left.
    list->tail = NULL;
  }
  else {                                // Otherwise just move forward.
    list->head = list->head->next;
    list->head->prev = NULL;
  }
  free( dying );                        // The node is no longer needed.
  return current;                       // Return the process.
}

void insertProcessInOrder( struct ProcessList * list,
			   struct Process * proc )
{
                                        // Create the new node.
  struct ProcessListNode * newNode = malloc( sizeof( struct ProcessListNode ) );
  newNode->next = NULL;
  newNode->data = proc;

  if( list->head == NULL ) {            // If it was empty, this is now the
    list->head = newNode;               //   first and last node.
    list->tail = newNode;
    newNode->prev = NULL;
  }
  else if( proc->units.head->data->mSecsLeft < 
	   list->head->data->units.head->data->mSecsLeft ) {
    newNode->next = list->head;         // If this has less time to completion
    list->head = newNode;               //   than the first, this is the new
    newNode->prev = NULL;               //   first.
  }
  else {                                // Otherwise find the proper place.
    struct ProcessListNode * current = list->head;
    while( current->next != NULL &&
	   current->next->data->units.head->data->mSecsLeft <=
	   proc->units.head->data->mSecsLeft ) {
      current = current->next;
    }
    if( current->next == NULL ) {
      current->next = newNode;
      newNode->prev = current;
      list->tail = newNode;
    }
    else {
      current->next->prev = newNode;
      newNode->next = current->next;
      newNode->prev = current;
      current->next = newNode;
    }
  }
}
