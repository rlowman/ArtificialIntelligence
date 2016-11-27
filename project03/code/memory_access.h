// File: memory_access.h
// Author: Chad Hogg
// Version: 2014-11-09
// This file is part of project 03 in CS364 at King's College in Fall 2014.
// It declares a data structure to represent an instance of a process wanting
//   to access a location in memory.

/**
 * An instance of a process accessing a memory location.
 **/
struct MemoryAccess {
  /**
   * The address that was accessed.
   **/
  unsigned long address;

  /**
   * Whether or not this access modified the location.
   **/
  bool isWrite;
};
