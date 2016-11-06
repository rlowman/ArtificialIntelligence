// File: schedule-simulator.c
// Author: Chad Hogg
// Simulates the scheduler in an operating system.
// This is not perfect in that it assumes all I/O interrupts take no time.
// Also, it assumes that all I/O blocks are to different devices, or are
//   otherwise perfectly parallelizable.  Still, it should be a reasonable
//   approximation.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "execution_unit.h"
#include "execution_unit_list.h"
#include "process.h"
#include "process_list.h"
#include "schedule_utilities.h"
#include "schedule_algorithms.h"



// Global variables that are used for all scheduling algorithms:

struct Process * running;               // A pointer to the currently running
                                        //   process (if there is one).

struct ProcessList waiting;             // A list of processes that have not
                                        // A list of processes that have not
                                        //   yet been created, ordered from
                                        //   closest to creation time to
                                        //   furthest from creation time.

struct ProcessList blocked;             // A list of processes currently in
                                        //   the blocked state, ordered from
                                        //   closest to unblocking to furthest
                                        //   from unblocking.

struct ProcessList finished;            // A queue of processes that have
                                        //   already terminated.

long tick;                              // The number of simulated milliseconds
                                        //   since the simulation started.

long idleTicks;                         // The number of milliseconds in which
                                        //   the CPU was unused.
long contextSwitchTicks;                // The number of milliseconds in which
                                        //   the CPU was doing a context
                                        //   switch.

int scheduler;                          // Which scheduling algorithm is in
                                        //   use.


// Here are the global variables specific to Round-Robin, though other
//   algorithms might end up using them as well:

int quantumRR;                          // The maximum amount of time that a
                                        //   process may run before being
                                        //   preempted in Round-Robin
                                        //   scheduling.

struct ProcessList readyRR;             // A queue of processes currently in
                                        //   the ready state.  This is used by
                                        //   the Round-Robin algorithm.  Other
                                        //   algorithms may use it as well, or
                                        //   may store the ready processes in
                                        //   some other data structure as
                                        //   appropriate.


/**
 * Process the command-line arguments received by the program.
 *
 * @param argc How many arguments there are total.
 * @param argv An array of arguments, including the program name.
 **/
void processArguments( int argc, char * argv[] );

/**
 * Print information about what happened in this simulation.
 * Should only be called after the simulation has completed.
 **/
void printStatistics();


int main( int argc, char * argv[] )
{
  running = NULL;                       // Initialize global variables.
  waiting.head = NULL;
  waiting.tail = NULL;
  blocked.head = NULL;
  blocked.tail = NULL;
  finished.head = NULL;
  finished.tail = NULL;
  tick = 0L;
  idleTicks = 0L;
  contextSwitchTicks = 0L;
  scheduler = 0;

  readyRR.head = NULL;                  // Globals specifically for the
  readyRR.tail = NULL;                  //   Round-Robin algorithm.
  quantumRR = 0;

  processArguments( argc, argv );

                                        // Create some processes and put them
                                        //   in the proper queues.
  setProcInitialPosition( createProcTypeA( "User's Editor", 40, 0 ),
			  tick, &readyRR, &waiting, scheduler );
  setProcInitialPosition( createProcTypeA( "Root's Editor", 50, 0 ),
			  tick, &readyRR, &waiting, scheduler );
  setProcInitialPosition( createProcTypeB( "Database Server", 60, 0 ),
			  tick, &readyRR, &waiting, scheduler );
  setProcInitialPosition( createProcTypeB( "Web Server", 70, 0 ),
			  tick, &readyRR, &waiting, scheduler );
  setProcInitialPosition( createProcTypeC( "User's Compiler", 15, 0 ),
			  tick, &readyRR, &waiting, scheduler );
  setProcInitialPosition( createProcTypeC( "Root's Compiler", 30, 0 ),
			  tick, &readyRR, &waiting, scheduler );
  setProcInitialPosition( createProcTypeD( "Update Checker", 10, 200 ),
			  tick, &readyRR, &waiting, scheduler );
  setProcInitialPosition( createProcTypeD( "Email Updater 1", 5, 500 ),
			  tick, &readyRR, &waiting, scheduler );
  setProcInitialPosition( createProcTypeD( "Email Updater 2", 5, 1000 ),
			  tick, &readyRR, &waiting, scheduler );


  while( !isReadyStateEmpty(&readyRR,   // The main simulation loop -- continue
			    scheduler)  //   while there are any unfinished
	 || running != NULL             //   processes.
	 || blocked.head != NULL        
	 || waiting.head != NULL ) {

    updateSingleProcess( running );     // Each process that was running,
    updateProcessesInList( &waiting );  //   blocked, or waiting to be created
    updateProcessesInList( &blocked );  //   is one msec closer to finishing
                                        //   its current execution unit.

    if( running != NULL &&              // If there was a running process and
	                                //   it has finished, move it to the
	                                //   appropriate state.
	running->units.head->data->mSecsLeft == 0 ) {
      moveFinishedRunningProcess( running,
				  &blocked,
				  &finished,
				  tick,
				  scheduler );
      running = NULL;                   // Now nothing is running.
    }

                                        // Move any processes that have
                                        //   finished waiting to start to the
                                        //   proper state.
    moveFinishedWaitingProcesses( &waiting,
				  &readyRR,
				  tick, scheduler );
                                        // Move any processes that have
                                        //   finished blocking to the ready
                                        //   state.
    moveFinishedBlockedProcesses( &blocked,
				  &readyRR,
				  tick, scheduler);

                                        // The scheduler should run, so that it
                                        //   can choose a new running process
                                        //   if there is none, or can preempt
                                        //   the running process if it desires.
    switch( scheduler ) {

    case SCHEDULER_ROUND_ROBIN:
      running = schedulePreemptiveRoundRobin( running, &readyRR, &blocked, &waiting, &tick, &contextSwitchTicks, quantumRR );
      break;

    case SCHEDULER_FIRST_COME_FIRST_SERVED:
      running = scheduleFirstComeFirstServed(running, &readyRR, &blocked, &waiting, &tick, &contextSwitchTicks);
      break;
    

    default:
      printf( "Error: Attempt to use an unrecognized scheduling algorithm.\n" );
      exit( 1 );
    }

    if( running == NULL ) {             // If the scheduler did not pick anyone
      idleTicks++;                      //   we are just waiting for someone to
    }                                   //   be ready.

    tick++;                             // Move on to the next simulated
                                        //   millisecond.
  }

  printStatistics();

  return 0;
}


void printStatistics() {
  long simulationEndTime = 0L;          // Some variables for keeping track of
  long numberOfProcesses = 0L;          //   our metrics.
  long totalExecutionTimes = 0L;
  long totalWeight = 0L;
  long weightedExecutionTimes = 0L;
  long totalInteractiveWaitTime = 0L;
  long totalInteractiveWaitTimeCount = 0L;
  long weightedTotalInteractiveWaitTime = 0L;
  long weightedTotalInteractiveWaitTimeCount = 0L;

  while( finished.head != NULL )  {     // Compute statistics about all of the
                                        //   finished processes.
    struct Process * tempProc = dequeueProcess( &finished );
    printf( "Process %s arrived at time %ld and finished at time %ld, for a turnaround of %ld.\n", tempProc->name, tempProc->startTime, tempProc->finishTime, tempProc->finishTime - tempProc->startTime );
    numberOfProcesses++;
    totalWeight += tempProc->priority;
    if( simulationEndTime < tempProc->finishTime )
      simulationEndTime = tempProc->finishTime;
    totalExecutionTimes += tempProc->finishTime - tempProc->startTime;
    weightedExecutionTimes += tempProc->priority * (tempProc->finishTime - tempProc->startTime );
    totalInteractiveWaitTime += tempProc->totalInteractiveWaitTime;
    weightedTotalInteractiveWaitTime += tempProc->priority * tempProc->totalInteractiveWaitTime;
    totalInteractiveWaitTimeCount += tempProc->numberOfInteractiveWaits;
    weightedTotalInteractiveWaitTimeCount += tempProc->priority * tempProc->numberOfInteractiveWaits;
    free( tempProc );
  }

  printf( "\n\n" );
  printf( "Throughput was %f jobs per second.\n", (double)numberOfProcesses / simulationEndTime * 1000 );
  printf( "Average turnaround was %f milliseconds.\n", (double)totalExecutionTimes / numberOfProcesses );
  printf( "Priority-weighted average turnaround was %f milliseconds.\n", (double)weightedExecutionTimes / totalWeight );
  printf( "Average response time was %f milliseconds.\n", (double)totalInteractiveWaitTime / totalInteractiveWaitTimeCount );
  printf( "Priority-weighted average response time was %f milliseconds.\n", (double)weightedTotalInteractiveWaitTime / weightedTotalInteractiveWaitTimeCount );
}

void processArguments( int argc, char * argv[] ) {
  // TODO: Update this for each new scheduling algorithm, accepting it as a
  //   command-line argument and also adding arguments for any options it may
  //   require.
  int i;
  for( i = 1; i < argc; i++ ) {
    if( strcmp( argv[i], "-s" ) == 0 ) {
      if( argc == i + 1 ) {
	printf( "Error: The -s argument must be followed by a scheduling algorithm name.\n" );
	exit( 1 );
      }
      i++;
      if( strcmp( argv[i], "RR" ) == 0 ||
	  strcmp( argv[i], "RoundRobin" ) == 0 ) {
	scheduler = SCHEDULER_ROUND_ROBIN;
      }
      else {
	printf( "Error: The -s argument must be followed by a scheduling algorithm name.\n" );
	exit( 1 );
      }
    }
    else if( strcmp( argv[i], "-q" ) == 0 ) {
      if( argc == i + 1 ) {
	printf( "Error: The -q argument must be followed by a positive integer.\n" );
	exit( 1 );
      }
      i++;
      quantumRR = atoi( argv[i] );
      if( quantumRR <= 0 ) {
	printf( "Error: The -q argument must be followed by a positive integer.\n" );
	exit( 1 );
      }
    }
    else if( strcmp( argv[i], "-h" ) == 0 || strcmp( argv[i], "--help" ) == 0 ) {
      printUsage();
      exit( 0 );
    }
    else {
      printf( "Error: Unrecognized option \"%s\".\n", argv[i] );
      exit( 1 );
    }
  }

  if( scheduler == 0 ) {
    printf( "Error: You must select a scheduling algorithm.\n" );
    exit( 1 );
  }
  if( scheduler == SCHEDULER_ROUND_ROBIN && quantumRR == 0 ) {
    printf( "Error: When using the Preemptive Round-Robin algorithm you must provide a quantum.\n" );
    exit( 1 );
  }
}

