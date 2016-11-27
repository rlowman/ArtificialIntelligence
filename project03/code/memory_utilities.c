// File: memory_utilities.c
// Author: Chad Hogg
// Version: 2014-11-09
// This file is part of project 03 in CS364 at King's College in Fall 2014.
// Defines auxilliary functions for a virtual memory simulator.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#include "page_table_entry.h"
#include "page_table.h"
#include "memory_access.h"
#include "memory_access_list.h"
#include "memory_utilities.h"

struct MemoryAccessList * readLogFile( char * filename ) {
  char header[3];                       // Either "R " or "W "
  char address[9];                      // An 8-digit hexadecimal number
  char newline[2];                      // "\n"
  char errorString[1024];
  int fileDescriptor;
  int readCount;
  int lines = 0;
  struct MemoryAccessList * list;       // The linked list we will return
  struct MemoryAccess * current;        // The access we are currently creating

  list = malloc( sizeof( struct MemoryAccessList ) );
  list->head = NULL;
  list->tail = NULL;

  memset( header, 0, 3 );
  memset( address, 0, 9 );
  memset( newline, 0, 2 );

  fileDescriptor = open( filename, O_RDONLY );
  if( fileDescriptor < 0 ) {
    sprintf( errorString, "Error while opening logfile \"%s\"", filename );
    perror( errorString );
    exit( 1 );
  }

  readCount = read( fileDescriptor,     // Read a header
		     header, 
		     2 );
  while( readCount == 2 ) {             // Continue while we found a header
    readCount = read( fileDescriptor,   // Read an address
		       address, 
		       8 );
    if( readCount != 8 ) {              // Error if we did not find one
      printf( "Error while reading line %d of logfile \"%s\": address not found.\n", lines, filename );
      exit( 1 );
    }
    readCount = read( fileDescriptor,   // Read the newline character
		       newline, 
		       1 );

    current = malloc( sizeof( struct MemoryAccess ) );
    errno = 0;
    current->address = strtoul( address, NULL, 16 );
    if( errno != 0 ) {
      perror( "Reading address" );
    }
    if( header[0] == 'W' ) {
      current->isWrite = true;
    }
    else {
      current->isWrite = false;
    }
			 
    enqueueMA( list, current );         // Add it to the end of the list
    readCount = read( fileDescriptor,   // Read the next line's header 
		       header, 
		       2 );
    lines++;
  }

  readCount = close( fileDescriptor );
  if( readCount != 0 ) {
    sprintf( errorString, "Error while closing logfile \"%s\"", filename );
    perror( errorString );
    exit( 1 );
  }

  return list;
}

void printUsage() {
  printf( "Usage: memory-simulator ALGORITHM PAGE_SIZE NUM_FRAMES FILENAME \n" );
  printf( "Simulates virtual memory for one process.\n\n" );
  printf( "\tALGORITHM must be one of the following values:\n" );
  printf( "\t\tRandom Optimal NRU FIFO SC LRU NFU Aging\n" );
  printf( "\tPAGE_SIZE is the number of kilobytes in each page\n" );
  printf( "\tNUM_FRAMES is the number of frames in physical memory available\n" );
  printf( "\tFILENAME is the path a file containing a log of memory accesses\n" );
}

void processArguments( int argc,
		       char * argv[],
		       int * algorithm,
		       struct PageTable * pageTable,
		       struct MemoryAccessList ** accesses ) {
  if( argc != 5 ) {
    printUsage();
    exit( 2 );
  }

  if( strcmp( argv[1], "Random" ) == 0 ) {
    *algorithm = ALGORITHM_RANDOM;
  }
  else if( strcmp( argv[1], "Optimal" ) == 0 ) {
    *algorithm = ALGORITHM_OPTIMAL;
  }
  else if( strcmp( argv[1], "NRU" ) == 0 ) {
    *algorithm = ALGORITHM_NRU;
  }
  else if( strcmp( argv[1], "FIFO" ) == 0 ) {
    *algorithm = ALGORITHM_FIFO;
  }
  else if( strcmp( argv[1], "SC" ) == 0 ) {
    *algorithm = ALGORITHM_SC;
  }
  else if( strcmp( argv[1], "LRU" ) == 0 ) {
    *algorithm = ALGORITHM_LRU;
  }
  else if( strcmp( argv[1], "NFU" ) == 0 ) {
    *algorithm = ALGORITHM_NFU;
  }
  else if( strcmp( argv[1], "Aging" ) == 0 ) {
    *algorithm = ALGORITHM_AGING;
  }
  else {
    printf( "Error: The first argument must be a page replacement algorithm name.\n" );
    printUsage();
    exit( 1 );
  }

  pageTable->pageSize = atoi( argv[2] );
  if( pageTable->pageSize <= 0 ) {
    printf( "Error: the second argument must be a positive number of KB per page.\n" );
    printUsage();
    exit( 1 );
  }
  pageTable->pageSize = pageTable->pageSize * 1024;

  pageTable->numFrames = atoi( argv[3] );
  if( pageTable->numFrames <= 0 ) {
    printf( "Error: the third argument must be a positive number of page frames available.\n" );
    printUsage();
    exit( 1 );
  }

  printf( "Loading log file \"%s\" ... ", argv[4] );
  *accesses = readLogFile( argv[4] );
  printf( "finished\n" );
}
