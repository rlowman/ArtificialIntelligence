// File: page_table_entry.h
// Author: Chad Hogg
// Version: 2014-11-09
// This file is part of project 03 in CS364 at King's College in Fall 2014.
// It declares a data structure to represent the metadata stored about one
//   memory page.

/**
 * One entry in a pseudo-page table.
 *
 * Unlike, perhaps, a real system, we only care to store information about the
 *   resident pages.  When one is evicted, whatever data we had about it is no
 *   longer needed.
 **/
struct PageTableEntry {
  // TODO: Add new fields to this as necessary for new algorithms

  /**
   * Which page is currently loaded into the frame.
   **/
  int pageNumber;

  /**
   * The 'R' bit -- whether or not referenced since last interrupt.
   **/
  int wasReferenced;

  /**
   * The 'M' bit -- whether or not modified since loaded.
   **/
  int wasModified;

  /**
   * When will this page next be used, for Optimal algorithm, cached for later.
   **/
  long nextAccess;

  /**
   * Counter used for NFU algorithm
   **/
  int nfuCounter;

   /**
    * Time compared in Fifo algorithm
    **/
  long fifoTime;

   /**
    * Index of queue for second-chance algorithm
    **/
  long scTime;

   /**
    * Time compared for lru algorithm
    **/
  long lruTime;

};

/**
 * (Re-)initialize a page table entry for a newly-loaded page.
 *
 * @param entry A pointer to the PTE to initialize.
 * @param pageNumber The page number being loaded into this frame.
 * @param loadTime The current time.
 **/
void initializePTE( struct PageTableEntry * entry,
		    int pageNumber,
		    long loadTime );
