// File: page_table_entry.c
// Author: Chad Hogg
// Version: 2014-11-09
// This file is part of project 03 in CS364 at King's College in Fall 2014.
// It defines functions that operate on PageTableEntry structures.

#include "page_table_entry.h"

void initializePTE( struct PageTableEntry * entry,
		    int pageNumber,
		    long loadTime ) {

  entry->pageNumber = pageNumber;
  entry->wasReferenced = 0;
  entry->wasModified = 0;
  entry->nextAccess = loadTime;
	entry->fifoTime = loadTime;
	entry->nfuCounter = 0;
	entry->scTime = loadTime;
	entry->lruTime = loadTime;
	entry->agingCounter = 0;
}
