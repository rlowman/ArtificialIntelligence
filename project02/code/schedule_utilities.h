// File: schedule_utilities.h
// Author: Chad Hogg
// Version: 2014-10-22
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It declares some functions useful for scheduler simulation.

/**
 * The number of milliseconds it takes to perform a context switch.
 **/
#define CONTEXT_SWITCH_TIME 5


/**
 * Print a message explaining how to use the scheduling simulator.
 **/
void printUsage();

/**
 * Insert a process into the ready state.
 * This should be overridden for each scheduling algorithm, since each may have
 *   their own data structure for storing ready processes.
 *
 * @param process A pointer to the process that has become ready.
 * @param ready A pointer to a list of processes in the ready state.
 * @param scheduler An integer specifying which scheduler should be tested.
 **/
void insertIntoReadyState( struct Process * process,
			   struct ProcessList * ready,
			   int scheduler );

/**
 * Test whether or not the ready state is empty (contains no processes).
 * This should be overridden for each scheduling algorithm, since each may have
 *   their own data structure for storing these processes.
 *
 * @param ready A pointer to the list of processes in the ready state.
 * @param scheduler An integer specifying which scheduler should be tested.
 * @return Whether or not there are no ready processes.
 **/
bool isReadyStateEmpty( struct ProcessList * ready,
			int scheduler);

/**
 * Put a new process into the appropriate data structure for the state it starts in.
 *
 * @param newProcess A pointer to the process that must be positioned.
 * @param tick The current simulation time counter.
 * @param ready A pointer to a list of processes in the ready state.
 * @param waiting A pointer to a list of processes in the waiting state.
 * @param scheduler An integer specifying which scheduler is being tested.
 **/
void setProcInitialPosition( struct Process * newProcess,
			     long tick,
			     struct ProcessList * ready,
			     struct ProcessList * waiting,
			     int scheduler );

/**
 * Decrement the time until its first execution unit completes for all processes in a list.
 *
 * @param list The list of processes to update.
 **/
void updateProcessesInList( struct ProcessList * list );

/**
 * Decrement the time until its first execution unit completes for a single process.
 *
 * @param process The process to update.  May be NULL to do nothing.
 **/
void updateSingleProcess( struct Process * process );

/**
 * Move the running process to a new queue if it is finished/blocked.
 *
 * @param process A pointer to the previously-running process, which is changing state.
 * @param blocked A pointer the list of blocked processes.
 * @param terminated A pointer to the list of terminated processes.
 * @param tick The current simulation time counter.
 * @param scheduler An integer specifying which scheduler is being tested.
 **/
void moveFinishedRunningProcess( struct Process * process,
				 struct ProcessList * blocked,
				 struct ProcessList * terminated,
				 long tick,
				 int scheduler );

/**
 * Move any processes that are finished waiting into the proper state.
 *
 * @param waiting A pointer to the list of waiting processes.
 * @param ready A pointer to the list of ready processes.
 * @param tick The current simulation time counter.
 * @param scheduler An integer specifying which scheduler is being tested.
 **/
void moveFinishedWaitingProcesses( struct ProcessList * waiting,
				   struct ProcessList * ready,
				   long tick,
				   int scheduler );

/**
 * Move any processes that are finished blocking into the proper state.
 *
 * @param blocked A pointer to the list of blocked processes.
 * @param ready A pointer to the list of ready processes.
 * @param tick The current simulation time counter.
 * @param scheduler An integer specifying which scheduler is being tested.
 **/
void moveFinishedBlockedProcesses( struct ProcessList * blocked,
				   struct ProcessList * ready,
				   long tick,
				   int scheduler );

/**
 * Mark the time needed to do a context switch.
 *
 * @param ready A pointer to the list of ready processes.
 * @param blocked A pointer to the list of blocked processes.
 * @param waiting A pointer to the list of waiting processes.
 * @param tick A pointer to the current simulation time counter.
 * @param contextSwitchTicks A pointer to the number of ticks wasted to context switches.
 * @param scheduler An integer specifying which scheduler is being tested.
 **/
void doContextSwitch( struct ProcessList * ready,
		      struct ProcessList * blocked,
		      struct ProcessList * waiting,
		      long * tick,
		      long * contextSwitchTicks,
		      int scheduler );

