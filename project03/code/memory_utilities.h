// File: memory_utilities.h
// Author: Chad Hogg
// Version: 2014-11-09
// This file is part of project 03 in CS364 at King's College in Fall 2014.
// Declares auxilliary functions for a virtual memory simulator.

// The various page replacement algorithms:

#define ALGORITHM_RANDOM 1
#define ALGORITHM_OPTIMAL 2
#define ALGORITHM_NRU 3
#define ALGORITHM_FIFO 4
#define ALGORITHM_SC 5
#define ALGORITHM_LRU 6
#define ALGORITHM_NFU 7
#define ALGORITHM_AGING 8

/**
 * Read a log file, creating a linked list of memory accesses from it.
 *
 * @param filename The name of a file to be processed to fill in the list.
 * @return A pointer to a linked list of MemoryAccess structures.
 **/
struct MemoryAccessList * readLogFile( char * filename );

/**
 * Print a message explaining how to use the program.
 **/
void printUsage();

/**
 * Process the command-line arguments received by the program.
 *
 * @param argc How many arguments there are in total.
 * @param argv An array of arguments, including the program name.
 * @param algorithm A pointer to where the algorithm should be chosen.
 * @param pageTable A pointer to the page table.
 * @param accesses A pointer to the list of accesses that should be processed.
 **/
void processArguments( int argc,
		       char * argv[],
		       int * algorithm,
		       struct PageTable * pageTable,
		       struct MemoryAccessList ** accesses );
