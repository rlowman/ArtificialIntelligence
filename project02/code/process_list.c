// File: process_list.c
// Author: Chad Hogg
// Version: 2014-10-21
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It contains definitions of functions for manipulating linked lists of
//   processes.

#include <stdlib.h>
#include <stdio.h>

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

void enqueueProcessMQ( struct ProcessList * list,
		     struct Process * proc,
		     int setQueueLevel)
{
                                        // Create the new node.
  struct ProcessListNode * newNode = malloc( sizeof( struct ProcessListNode ) );
  newNode->next = NULL;
  proc->queueLevel = setQueueLevel;
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
  struct ProcessListNode * shortestNode = current;
  struct Process * returnValue;
  if( list->head == list->tail ) {
    printf("here7\n");
    returnValue = current->data;
    printf("here8\n");
    list->head = NULL;
    printf("here9\n");
    list->tail = NULL;
    printf("here10\n");
    free(current);
    printf("here11\n");
  }
  else {                                
    tempBeforeNode = current;
    current = current->next;
    printf("1\n");
    printf("2\n");
    while(current != NULL) {
      if(current->data->time < shortest) {
	printf("3\n");
	shortest = current->data->time;
	printf("4\n");
	shortestNode = current;
	printf("5\n");
	beforeShortest = tempBeforeNode;
	printf("6\n");
      }
      tempBeforeNode = current;
      printf("7\n");
      current = current->next;
      printf("8\n");
    }

    if(list->head == shortestNode) {
      printf("9\n");
      list->head = shortestNode->next;
    }
    else if(list->tail == shortestNode) {
      printf("10\n");
      list->tail = beforeShortest;
      beforeShortest->next = NULL;
    }
    else {
      printf("11\n");
      beforeShortest->next = shortestNode->next;
    }
    printf("12\n");
    returnValue = shortestNode->data;
    printf("13\n");
    free(shortestNode);
    printf("14\n");
  }                       
  return returnValue;                      
}

struct Process * dequeueProcessHP(struct ProcessList * list) {
  printf("The contents of the list:\n");
  struct ProcessListNode * mover = list->head;
  while(mover != NULL){
    printf("\t%s %d\n", mover->data->name, mover->data->priority);
    mover = mover->next;
  }
  struct ProcessListNode * current = list->head;
  struct ProcessListNode * tempBeforeNode;
  struct ProcessListNode * beforeHighest;
  int highest = current->data->priority;
  struct ProcessListNode * highestNode = current;
  struct Process * returnValue;
  if( list->head == list->tail ) {
    returnValue = current->data;
    list->head = NULL;                  
    list->tail = NULL;
    free(current);
  }
  else {                                
    tempBeforeNode = current;
    current = current->next;
    while(current != NULL) {
      if(current->data->priority > highest) {
	highest = current->data->priority;
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
      beforeHighest->next = NULL;
    }
    else {
      beforeHighest->next = highestNode->next;
    }
    returnValue = highestNode->data;
    free(highestNode);
  }
  return returnValue;
}

struct Process * dequeueProcessMQ( struct ProcessList * list, int numberOfQueues )
{
  struct ProcessListNode * current = list->head;
  struct ProcessListNode * tempBeforeNode;
  struct ProcessListNode * beforePopNode;
  int currentQueue = numberOfQueues;
  struct ProcessListNode * popNode;
  struct Process * returnValue;
  if( list->head == list->tail ) {
    list->head = NULL;                  
    list->tail = NULL;
    returnValue = current->data;
    free(current);
  }
  else {
    int foundNode = 0;
    while(foundNode == 0) {
      if(current->data->queueLevel == currentQueue) {
	popNode = current;
	foundNode = 1;
      }
      else {
	tempBeforeNode = current;
	current = current->next;
	while(current != NULL && foundNode == 0) {
	  if(current->data->queueLevel == currentQueue) {
	    popNode = current;
	    beforePopNode = tempBeforeNode;
	    foundNode = 1;
	  }
	  current = current->next;
	}
	current = list->head;
	currentQueue--;
      }
    }

    if(list->head == popNode) {
      list->head = popNode->next;
    }
    else if(list->tail == popNode) {
      list->tail = beforePopNode;
      beforePopNode->next = NULL;
    }
    else {
      beforePopNode->next = popNode->next;
    }
    returnValue = popNode->data;
    free(popNode);
  }                       
  return returnValue;
}

struct Process * dequeueProcessLJF( struct ProcessList * list )
{
  struct ProcessListNode * current = list->head;
  struct ProcessListNode * tempBeforeNode;
  struct ProcessListNode * beforeLongest;
  int longest = current->data->time;
  struct ProcessListNode * longestNode = current;
  struct Process * returnValue;
  if( list->head == list->tail ) {
    list->head = NULL;                  
    list->tail = NULL;
    returnValue = current->data;
    free(current);
  }
  else {                                
    tempBeforeNode = current;
    current = current->next;
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
      beforeLongest->next = NULL;
    }
    else {
      beforeLongest->next = longestNode->next;
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
