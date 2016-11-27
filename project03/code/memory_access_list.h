// File: memory_access_list.h
// Author: Chad Hogg
// Version: 2014-11-09
// This file is part of project 03 in CS364 at King's College in Fall 2014.
// It declares a data structure to represent a linked list of MemoryAccess
//   structures.

/**
 * A node in a (singly) linked list of MemoryAccess structures.
 **/
struct MemoryAccessListNode {
  /**
   * A pointer to the MemoryAccess structure stored in this node.
   **/
  struct MemoryAccess * data;

  /**
   * A pointer to the next node in the list (NULL if none).
   **/
  struct MemoryAccessListNode * next;
};

/**
 * A (singly) linked list of MemoryAccess structures.
 **/
struct MemoryAccessList {
  /**
   * A pointer to the first node in the list (NULL if none).
   **/
  struct MemoryAccessListNode * head;

  /**
   * A pointer to the last node in the list (NULL if none).
   **/
  struct MemoryAccessListNode * tail;
};

/**
 * Add a new MemoryAccess to the end of a linked list of them.
 *
 * @param list A pointer to a MemoryAccessList into which we are inserting.
 * @param newItem A pointer to a new MemoryAccess to append to the list.
 **/
void enqueueMA( struct MemoryAccessList * list,
		struct MemoryAccess * newItem );

/**
 * Remove the first MemoryAccess from a linked list of them.
 *
 * @param list A pointer to a MemoryAccessList from which we are removing.
 * @return A pointer to what had been the first MemoryAccess in the list.
 **/
struct MemoryAccess * dequeueMA( struct MemoryAccessList * list );
