// File: process_list.h
// Author: Chad Hogg
// Version: 2014-10-21
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It contains declarations of types for linked lists of processes and
//   functions for manipulating them.

/**
 * A node in a doubly-linked list of processes.
 **/
struct ProcessListNode
{
  /**
   * A pointer to the payload of this node.
   **/
  struct Process * data;

  /**
   * A pointer to the node that follows this one in the list.  (NULL if none.)
   **/
  struct ProcessListNode * next;

  /**
   * A pointer to the node that preceeds this one in the list. (NULL if none.)
   **/
  struct ProcessListNode * prev;
};

/**
 * A doubly-linked list of processes.
 **/
struct ProcessList {
  /**
   * A pointer to the first node in the list (NULL if none).
   **/
  struct ProcessListNode * head;

  /**
   * A pointer to the last node in the list (NULL if none).
   **/
  struct ProcessListNode * tail;
};

/**
 * Add a new process to the end of a list of processes.
 *
 * @param list A pointer to the list in which it is being enqueued.
 * @param proc A pointer to a process that should be added to the list.
 **/
void enqueueProcess( struct ProcessList * list,
		     struct Process * proc );


/**
 * Remove the first process from a list of processes.
 *
 * @param list A pointer to the list from which we are dequeing.
 **/
struct Process * dequeueProcess( struct ProcessList * list );

/**
 * Remove the process with the shortest job.
 *
 * @param list A pointer to the list from which we are dequeing.
 **/
struct Process * dequeueProcessSJF( struct ProcessList * list );

/**
 * Remove the process with the highest priority.
 *
 * @param list A pointer to the list from which we are dequeing.
 **/
struct Process * dequeueProcessHP( struct ProcessList * list );

/**
 * Remove the process in Multiple Queues order.
 *
 * @param list A pointer to the list from which we are dequeing.
 **/
struct Process * dequeueProcessMQ( struct ProcessList * list );

/**
 * Remove the first process from a list of processes.
 *
 * @param list A pointer to the list from which we are dequeing.
 **/
struct Process * dequeueProcessLJF( struct ProcessList * list );


/**
 * Insert a process into a list that is ordered by time remaining in current execution unit.
 *
 * @param begin A pointer to the list into which it should be inserted.
 * @param proc A pointer to a process to insert into the ordered list.
 **/
void insertProcessInOrder( struct ProcessList * list,
			   struct Process * proc );

