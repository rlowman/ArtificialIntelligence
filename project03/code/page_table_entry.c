// File: page_table_entry.c
// Author: Chad Hogg
// Version: 2014-11-09
// This file is part of project 03 in CS364 at King's College in Fall 2014.
// It defines functions that operate on PageTableEntry structures.

#include "page_table_entry.h"

void initializePTE( struct PageTableEntry * entry, 
		    int pageNumber, 
		    int loadTime ) {

  // TODO: Add initialization here of new fields added to struct PageTableEntry
  //   This may require adding new parameters to the function.
  entry->pageNumber = pageNumber;
  entry->wasReferenced = 0;
  entry->wasModified = 0;
  entry->nextAccess = loadTime;
}
