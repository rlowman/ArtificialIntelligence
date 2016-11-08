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

struct Process * dequeueProcessSJF( struct ProcessList * list )
{
  struct ProcessListNode * current = list->head;
  struct ProcessListNode * tempBeforeNode;
  struct ProcessListNode * beforeShortest;
  int shortest = current->data->time;
  struct ProcessListNode * shortestNode;
  struct Process * returnValue;
  if( list->head == list->tail ) {
    list->head = NULL;                  
    list->tail = NULL;
    returnValue = current->data;
    free(current);
  }
  else {                                
    current = current->next;
    tempBeforeNode = current;
    while(current != NULL) {
      if(current->data->time < shortest) {
	shortest = current->data->time;
	shortestNode = current;
	beforeShortest = tempBeforeNode;
      }
      tempBeforeNode = current;
      current = current->next;
    }

    if(list->head == shortestNode) {
      list->head = shortestNode->next;
    }
    else if(list->tail == shortestNode) {
      list->tail = beforeShortest;
    }
    else {
      beforeShortest->next = shortestNode->next;
    }
    returnValue = shortestNode->data;
    free(shortestNode);
  }                       
  return returnValue;                      
}

struct Process * dequeueProcessHP(struct ProcessList * list) {
  struct ProcessListNode * current = list->head;
  struct ProcessListNode * tempBeforeNode;
  struct ProcessListNode * beforeHighest;
  int highest = current->data->priority;
  struct ProcessListNode * highestNode;
  struct Process * returnValue;
  if( list->head == list->tail ) {
    list->head = NULL;                  
    list->tail = NULL;
    returnValue = current->data;
    free(current);
  }
  else {                                
    current = current->next;
    tempBeforeNode = current;
    while(current != NULL) {
      if(current->data->priority > highest) {
	highest = current->data->time;
	highestNode = current;
	beforeHighest = tempBeforeNode;
      }
      tempBeforeNode = current;
      current = current->next;
    }

    if(list->head == highestNode) {
      list->head = highestNode->next;
    }
    else if(list->tail == highestNode) {
      list->tail = beforeHighest;
    }
    else {
      beforeHighest->next = highestNode->next;
    }
    returnValue = highestNode->data;
    free(highestNode);
  }
  return returnValue;
}

struct Process * dequeueProcessMQ( struct ProcessList * list )
{
  struct Process * current = list->head->data;
  struct Process * returnValue;
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
  return returnValue;                       // Return the process.
}

struct Process * dequeueProcessLJF( struct ProcessList * list )
{
  struct ProcessListNode * current = list->head;
  struct ProcessListNode * tempBeforeNode;
  struct ProcessListNode * beforeLongest;
  int longest = current->data->time;
  struct ProcessListNode * LongestNode;
  struct Process * returnValue;
  if( list->head == list->tail ) {
    list->head = NULL;                  
    list->tail = NULL;
    returnValue = current->data;
    free(current);
  }
  else {                                
    current = current->next;
    tempBeforeNode = current;
    while(current != NULL) {
      if(current->data->time > longest) {
	longest = current->data->time;
	longestNode = current;
	beforeLongest = tempBeforeNode;
      }
      tempBeforeNode = current;
      current = current->next;
    }

    if(list->head == longestNode) {
      list->head = longestNode->next;
    }
    else if(list->tail == longestNode) {
      list->tail = beforeLongest;
    }
    else {
      beforeLongest->next = LongestNode->next;
    }
    returnValue = longestNode->data;
    free(longestNode);
  }                       
  return returnValue;                      
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
