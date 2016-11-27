// File: page_table.h
// Author: Chad Hogg
// Version: 2014-11-09
// This file is part of project 03 in CS364 at King's College in Fall 2014.
// It declares a data structure to represent a page table.

/**
 * A page table for virtual memory.
 * Technically, this is not accurate.  A real page table would have an entry
 *   for each page, regardless of whether or not that page was resident.  But
 *   this has one entry per frame, and does not store information about
 *   non-resident pages.  That is because we do not need to store anything
 *   about non-resident pages for this simulation.
 **/
struct PageTable {
  /**
   * The actual table, which is a dynamically-allocated array.
   **/
  struct PageTableEntry * frames;

  /**
   * The size of each page, in bytes.
   **/
  long pageSize;

  /**
   * The number of frames of physical memory available.
   **/
  long numFrames;
};
