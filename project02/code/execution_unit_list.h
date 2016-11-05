// File: execution_unit_list.h
// Author: Chad Hogg
// Version: 2014-10-21
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It contains declarations of types for linked lists of execution units and
//   functions for manipulating them as a FIFO queue.

/**
 * A node in a doubly-linked list of process execution components.
 **/
struct ExecutionUnitListNode {

  /**
   * A pointer to the payload of this node.
   **/
  struct ExecutionUnit * data;

  /**
   * A pointer to the node that follows this one in the list.
   * This will be NULL for the first node in the list.
   **/
  struct ExecutionUnitListNode * next;

  /**
   * A pointer to the node that preceeds this one in the list.
   * This will be NULL for the last node in the list.
   **/
  struct ExecutionUnitListNode * prev;
};

/**
 * A doubly-linked list of execution units.
 **/
struct ExecutionUnitList {
  /**
   * A pointer to the first node in the list (NULL if none).
   **/
  struct ExecutionUnitListNode * head;

  /**
   * A pointer to the last node in the list (NULL if none).
   **/
  struct ExecutionUnitListNode * tail;
};

/**
 * Add a new execution unit to a queue of them.
 *
 * @param list A pointer to a list of execution units.
 * @param mSecs The number of milliseconds for the execution unit to add.
 * @param type The type of the execution unit to add.
 **/
void enqueueExecutionUnit( struct ExecutionUnitList * list,
			   int mSecs,
			   int type );


/**
 * Remove an execution unit from a queue of them.
 *
 * @param list A pointer to the list that should be modified.  This must not
 *   be empty.
 **/
void dequeueExecutionUnit( struct ExecutionUnitList * list );
