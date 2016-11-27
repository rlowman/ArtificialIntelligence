// File: memory_simulator.c
// Author: Chad Hogg
// Version: 2014-11-09
// This file is part of project 03 in CS364 at King's College in Fall 2014.
// Simulates memory paging in an operating system.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

#include "page_table_entry.h"
#include "page_table.h"
#include "memory_access.h"
#include "memory_access_list.h"
#include "memory_utilities.h"


#define CLOCK_INTERRUPT 100             // How often the 'R' bit gets reset
#define DEBUG false                     // Set to true for helpful messages
#define INVALID -1                      // How we represent no (int) data


/**
 * Do whatever processing is needed on every clock interrupt.
 *
 * @param pageTable A pointer to the page table.
 **/
void onClockInterrupt( struct PageTable * pageTable );

/**
 * Do whatever processing is needed after every access.
 *
 * @param pageTable A pointer to the page table.
 * @param used_frame Which frame holds the page that was accessed.
 * @param is_write Whether or not this access was a write.
 **/
void onEveryAccess( struct PageTable * pageTable,
		    int used_frame,
		    bool is_write );

/**
 * Select a frame for eviction.
 *
 * @param algorithm Which algorithm is in use.
 * @param pageTable A pointer to the page table.
 * @param accesses A pointer to the list of future memory accesses.
 * @param time The number of cycles that have already passed.
 **/
int selectEvictionFrame( int algorithm,
			 struct PageTable * pageTable,
			 struct MemoryAccessList * accesses,
			 long time );


int selectFrameRandom( struct PageTable * pageTable );
int selectFrameOptimal( struct PageTable * pageTable,
			struct MemoryAccessList * accesses,
			long time );
int selectFrameNRU( struct PageTable * pageTable );
int selectFrameFIFO( struct PageTable * pageTable );
int selectFrameSecondChance( struct PageTable * pageTable );
int selectFrameLRU( struct PageTable * pageTable );
int selectFrameNFU( struct PageTable * pageTable );
int selectFrameAging( struct PageTable * pageTable );



int main( int argc, char * argv[] ) {
  struct PageTable pageTable;           // The page table.

  struct MemoryAccessList * accesses;   // The list of not-yet simulated
                                        //   memory accesses.

  int algorithm;                        // The algorithm to use

  long time;                            // How many clock cycles have gone by

  struct MemoryAccess * current;        // The access we are working with now
  int i;
  int foundFrame;                       // The frame # we are working with now
  int numFaults, numWrites;             // Results

  accesses = NULL;                      // Initialize everything to empty
  time = 0;
  numFaults = 0;
  numWrites = 0;

  processArguments( argc, argv, &algorithm, &pageTable, &accesses );
  pageTable.frames = malloc( pageTable.numFrames * sizeof(struct PageTableEntry) );
  for( i = 0; i < pageTable.numFrames; i++ ) {
    initializePTE( &pageTable.frames[i], INVALID, INVALID );
  }


  printf( "Running simulation ...\n" );

  while( accesses->head != NULL ) {     // Continue for all memory accesses

    current = dequeueMA( accesses );    // Get the next memory access

    long current_page = current->address// Find the page containing the
      / pageTable.pageSize;             //   current address

    foundFrame = INVALID;               // Look for the needed page in the
    i = 0;                              //   page table
    while( i < pageTable.numFrames && foundFrame == INVALID ) {
      if( current_page == pageTable.frames[i].pageNumber ) {
	foundFrame = i;
      }
      i++;
    }

    if( foundFrame == INVALID ) {       // If not found, we have a page fault
      numFaults++;

      i = 0;                            // Look for an already empty frame
      while( i < pageTable.numFrames && foundFrame == INVALID ) {
	if( pageTable.frames[i].pageNumber == INVALID ) {
	  foundFrame = i;
	}
	i++;
      }

      if( foundFrame == INVALID ) {     // If we did not fine one, evict a page
	foundFrame = selectEvictionFrame( algorithm, &pageTable, accesses, time );
	if( pageTable.frames[foundFrame].wasModified ) {
	  numWrites++;                  // If modified, we write it back
	}
	if( DEBUG ) {
	  printf( "Evicting page %d from frame %d to insert page %ld (for address %ld)\n", pageTable.frames[foundFrame].pageNumber, foundFrame, current_page, current->address );
	}
      }
      else {                            // Otherwise just take the empty frame
	if( DEBUG ) {
	  printf( "Putting page %ld in empty frame %d (for address %ld)\n", current_page, foundFrame, current->address );
	}
      }

      initializePTE( &pageTable.frames[foundFrame],
		     current_page,      // Initialize the newly loaded page
		     time );
    }
    else {
      if( DEBUG ) {
	printf( "Found address %ld in page %d in frame %d without a page fault.\n", current->address, pageTable.frames[foundFrame].pageNumber, foundFrame );
      }
    }

    onEveryAccess( &pageTable,          // Update the page table as might be
		   foundFrame,          //   necessary for the page we
		   current->isWrite );  //   accessed

    if( time % CLOCK_INTERRUPT == 0 ) { // If it is time for a clock
      onClockInterrupt( &pageTable );   //   interrupt, do one
    }

    free( current );                    // We are finished with this access

    time++;                             // Move forward in time
  }

  printf( "There were %d page faults and %d writes.\n", numFaults, numWrites );

  free( pageTable.frames );
  return 0;
}

void onClockInterrupt( struct PageTable * pageTable ) {
  // TODO: Add processing to this function as necessary for new algorithms
  int i;

  // Clear the 'R' bit for all pages
  for( i = 0; i < pageTable->numFrames; i++ ) {
    pageTable->frames[i].wasReferenced = 0;
  }
}

void onEveryAccess( struct PageTable * pageTable,
		    int used_frame,
		    bool is_write ) {
  // TODO: Add processing to this function as necessary for new algorithms.

  // Turn 'R' bit and perhaps 'M' bit on for that frame
  pageTable->frames[used_frame].wasReferenced = 1;
  if( is_write ) {
    pageTable->frames[used_frame].wasModified = 1;
  }
}


int selectEvictionFrame( int algorithm,
			 struct PageTable * pageTable,
			 struct MemoryAccessList * accesses,
			 long time )
{
  switch( algorithm ) {
  case ALGORITHM_RANDOM:
    return selectFrameRandom( pageTable );
  case ALGORITHM_OPTIMAL:
    return selectFrameOptimal( pageTable, accesses, time );
  case ALGORITHM_NRU:
    return selectFrameNRU( pageTable );
  case ALGORITHM_FIFO:
    return selectFrameFIFO( pageTable );
  case ALGORITHM_SC:
    return selectFrameSecondChance( pageTable );
  case ALGORITHM_LRU:
    return selectFrameLRU( pageTable );
  case ALGORITHM_NFU:
    return selectFrameNFU( pageTable );
  case ALGORITHM_AGING:
    return selectFrameAging( pageTable );
  default:
    printf( "Error: unrecognized algorithm in selectEvictionFrame().\n" );
    exit( 2 );
  }
}

int selectFrameRandom( struct PageTable * pageTable )
{
  return rand() % pageTable->numFrames;
}


int selectFrameOptimal( struct PageTable * pageTable,
			struct MemoryAccessList * accesses,
			long time )
{
  int longest = 0;                      // Which frame contains the page that
                                        //   will not be used for the longest
                                        //   time
  int i;
  for( i = 0;                           // Loop through entire page table
       i < pageTable->numFrames;
       i++ ) {

    if( pageTable->frames[i].nextAccess // The cached "next access" time is
	> INVALID                       //   past, so find the next one
	&& pageTable->frames[i].nextAccess < time ) {

                                        // Look through remaining memory
                                        //   accesses until we find one on this
                                        //   page
      struct MemoryAccessListNode * current = accesses->head;
      int count = 0;
      while( current != NULL && current->data->address / pageTable->pageSize != pageTable->frames[i].pageNumber ) {
	current = current->next;
	count++;
      }

      if( current == NULL )             // If none found, we never access again
	pageTable->frames[i].nextAccess = INVALID;
      else                              // Otherwise record when it will be
	pageTable->frames[i].nextAccess = time + count;
    }

    if( pageTable->frames[i].nextAccess == INVALID ) {
      return i;                         // If this page will never be accessed
    }                                   //   again, just evict it

    // Otherwise find which will go the longest before use
    if( pageTable->frames[i].nextAccess >
	pageTable->frames[longest].nextAccess )
      longest = i;
  }
  return longest;
}

int selectFrameNRU( struct PageTable * pageTable )
{
  int found = 0;
	for(int i = 0; i < pageTable->numFrames; i ++) {
		PageTableEntry * temp = pageTable->frames[i];
		if(temp->wasReferenced == 0 && temp->wasModified == 0) {
			found ++;
		}
	}
	if(found < 1) {
		for(int i = 0; i < pageTable->numFrames; i ++) {
			PageTableEntry * temp = pageTable->frames[i];
			if(temp->wasReferenced == 0 && temp->wasModified == 1) {
				found ++;
			}
		}
	}
	if(found < 1) {
		for(int i = 0; i < pageTable->numFrames; i ++) {
			PageTableEntry * temp = pageTable->frames[i];
			if(temp->wasReferenced == 1 && temp->wasModified == 0) {
				found ++;
			}
		}
	}
	if(found < 1) {
		for(int i = 0; i < pageTable->numFrames; i ++) {
			PageTableEntry * temp = pageTable->frames[i];
			if(temp->wasReferenced == 1 && temp->wasModified == 1) {
				found ++;
			}
		}
	}
	int r = rand() % found;
	int returnValue = -1;
	bool spot = false;
	int count = 0;
	int i = 0;
	while(found < 1 && i < pageTable->numFrames) {
		PageTableEntry * temp = pageTable->frames[i];
		if(temp->wasReferenced == 0 && temp->wasModified == 0) {
			count ++;
		}
	}
	i = 0;
	while(found < 1 && i < pageTable->numFrames) {
		PageTableEntry * temp = pageTable->frames[i];
		if(temp->wasReferenced == 0 && temp->wasModified == 1) {
			count ++;
		}
	}
	i = 0;
	while(found < 1 && i < pageTable->numFrames) {
		PageTableEntry * temp = pageTable->frames[i];
		if(temp->wasReferenced == 1 && temp->wasModified == 0) {
			count ++;
		}
	}
	i = 0;
	while(found < 1 && i < pageTable->numFrames) {
		PageTableEntry * temp = pageTable->frames[i];
		if(temp->wasReferenced == 1 && temp->wasModified == 1) {
			count ++;
		}
	}
  return returnValue;
}

int selectFrameFIFO( struct PageTable * pageTable )
{

  exit( 4 );
}

int selectFrameSecondChance( struct PageTable * pageTable )
{
  // TODO: Write this function
  printf( "Error: The SC algorithm has not yet been implemented.\n" );
  exit( 4 );
}

int selectFrameLRU( struct PageTable * pageTable )
{
  // TODO: Write this function
  printf( "Error: The LRU algorithm has not yet been implemented.\n" );
  exit( 4 );
}

int selectFrameNFU( struct PageTable * pageTable )
{
  // TODO: Write this function
  printf( "Error: The NFU algorithm has not yet been implemented.\n" );
  exit( 4 );
}

int selectFrameAging( struct PageTable * pageTable )
{
  // TODO: Write this function
  printf( "Error: The Aging algorithm has not yet been implemented.\n" );
  exit( 4 );
}
