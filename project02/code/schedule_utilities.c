// File: schedule_utilities.c
// Author: Chad Hogg
// Version: 2014-10-22
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It defines some functions used for scheduler simulation.

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "execution_unit.h"
#include "execution_unit_list.h"
#include "process.h"
#include "process_list.h"
#include "schedule_utilities.h"
#include "schedule_algorithms.h"


void printUsage() {
  // TODO: Update this for each new scheduling algorithm, listing it as an
  //   option and explaining any other arguments that may be necessary for it.
  printf( "Usage: schedule-simulator -s ALGORITHM [-q QUANTUM]\n" );
  printf( "Simulates a scheduling algorithm on a process load.\n\n" );
  printf( "Allowable algorithms:\n" );
  printf( "\tRR or RoundRobin\tPreemptive Round-Robin\n" );
  printf( "\n" );
  printf( "A quantum must be specified if the Preemptive Round-Robin algorithm is selected.\n" );
}

void insertIntoReadyState( struct Process * process,
			   struct ProcessList * ready,
			   int scheduler ) {
  // TODO: For every new scheduler added, update this function.
  switch( scheduler ) {

  case SCHEDULER_ROUND_ROBIN:           // Round-Robin just operates as a FIFO.
    enqueueProcess( ready, process );
    break;

    

  default:                              // An unsupported scheduler is in use.
    printf( "Error: Attempt to insert into ready state using unknown scheduler.\n" );
    exit( 1 );
  }
}

bool isReadyStateEmpty( struct ProcessList * ready,
			int scheduler ) {
  // TODO: For every new scheduler added, update this function.
  switch( scheduler ) {

  case SCHEDULER_ROUND_ROBIN:           // Round-Robin just checks if queue is
    return ready->head == NULL;         //   empty.

  default:                              // Unknown scheduler.
    printf( "Error: Attempt to check for emptiness of the ready state using unknown scheduler.\n" );
    exit( 1 );
  }
}



void setProcInitialPosition( struct Process * newProcess,
			     long tick,
			     struct ProcessList * ready,
			     struct ProcessList * waiting,
			     int scheduler ) {
  newProcess->startTime = tick;         // The process is starting right now.
                                        // Time until first interactive block
                                        //   counts toward response time.
  newProcess->lastInteractiveBlockEnd = tick;

                                        // If the process has no execution
                                        //   units, it terminates immediately.
                                        // (This should not happen.)
  if( newProcess->units.head == NULL ) {
    printf( "Error: process %s has nothing to do.\n", newProcess->name );
    exit( 1 );
  }
                                        // If the first thing the process needs
                                        //   to do is use the CPU, it goes into
                                        //   the ready state.
  else if( newProcess->units.head->data->type == TYPE_PROCESSING ) {
    insertIntoReadyState( newProcess, ready, scheduler );
  }
                                        // If the first thing the process needs
                                        //   to do is some I/O, it goes into
                                        //   the blocked state.
                                        // (This should not happen.)
  else if( newProcess->units.head->data->type == TYPE_BLOCKED ||
	   newProcess->units.head->data->type == TYPE_INTERACTIVE ) {
    printf( "Error: process %s starts blocked.\n", newProcess->name );
    exit( 1 );
  }
                                        // If the process is waiting to be
                                        //   started, put it in a list of those
                                        //   processes.  When it is finished
                                        //   waiting this will be called again.
  else if( newProcess->units.head->data->type == TYPE_NOT_YET_STARTED ) {
    insertProcessInOrder( waiting, newProcess );
  }
                                        // Those are the only supported types
                                        //   of execution units.
  else {
    printf( "Error: process %s has an execution unit of unknown type.\n", newProcess->name );
    exit( 1 );
  }
}

void updateProcessesInList( struct ProcessList * list ) {
  struct ProcessListNode * tempNode;
  tempNode = list->head;
  while( tempNode != NULL ) {           // Loop through entire list.
    tempNode->data->units.head->data->mSecsLeft--;
    tempNode = tempNode->next;
  }
}

void updateSingleProcess( struct Process * process ) {
  if( process != NULL ) {
    process->units.head->data->mSecsLeft--;
  }

}

void moveFinishedRunningProcess( struct Process * process,
				 struct ProcessList * blocked,
				 struct ProcessList * terminated,
				 long tick,
				 int scheduler ) {
  if( process != NULL && process->units.head->data->mSecsLeft == 0 ) {
                                        // The process is finished this unit.
    dequeueExecutionUnit( &process->units );
                                        // If there are more, it moves to
                                        //   another state.
    if( process->units.head != NULL ) {
      if( process->units.head->data->type == TYPE_PROCESSING ) {
	printf( "Error: process %s had two processing sections in a row.\n", process->name );
	exit( 1 );
      }
      else if( process->units.head->data->type == TYPE_BLOCKED ) {
	insertProcessInOrder( blocked, process );
      }
      else if( process->units.head->data->type == TYPE_INTERACTIVE ) {
	insertProcessInOrder( blocked, process );
	                                // If we started blocking on an
	                                //   interactive device, we have
	                                //   become ready to the user.
	process->totalInteractiveWaitTime += (tick - process->lastInteractiveBlockEnd);
	process->numberOfInteractiveWaits++;
      }
      else if( process->units.head->data->type == TYPE_NOT_YET_STARTED ) {
	printf( "Error: process %s went to not yet started after already running.\n", process->name );
	exit( 1 );
      }
      else {
	printf( "Error: unknown execution unit type %d\n", process->units.head->data->type );
	exit( 1 );
      }
    }
    else {                              // Otherwise, it is finished.
      process->finishTime = tick;
      process->totalInteractiveWaitTime += (tick - process->lastInteractiveBlockEnd);
      process->numberOfInteractiveWaits++;
      enqueueProcess( terminated, process );
    }
  }
  else {
    printf( "Error: You tried to move the running process at an improper time.\n" );
    exit( 1 );
  }
}

void moveFinishedWaitingProcesses( struct ProcessList * waiting,
				   struct ProcessList * ready,
				   long tick,
				   int scheduler ) {
  while( waiting->head != NULL && 
	 waiting->head->data->units.head->data->mSecsLeft == 0 ) {
    struct Process * starting = dequeueProcess( waiting );
    dequeueExecutionUnit( &starting->units );
    setProcInitialPosition( starting, tick, ready, waiting, scheduler );
  }
}

void moveFinishedBlockedProcesses( struct ProcessList * blocked,
				   struct ProcessList * ready,
				   long tick,
				   int scheduler ) {
  struct Process * unblocked;
  while( blocked->head != NULL && 
	 blocked->head->data->units.head->data->mSecsLeft == 0 ) {
                                        // We found a process that is finished
                                        //   being blocked.
    unblocked = dequeueProcess( blocked );
                                        // If this was an interactive block
                                        //   it affects response time.
    if( unblocked->units.head->data->type == TYPE_INTERACTIVE ) {
      unblocked->lastInteractiveBlockEnd = tick;
    }
    dequeueExecutionUnit( &unblocked->units );
    if( unblocked->units.head == NULL ) {
      printf( "Error: process %s finished while blocked.\n", unblocked->name );
      exit( 1 );
    }
    else if( unblocked->units.head->data->type == TYPE_BLOCKED ||
	     unblocked->units.head->data->type == TYPE_INTERACTIVE ) {
      printf( "Error: process %s had two I/O blocks in a row.\n", unblocked->name );
      exit( 1 );
    }
    else if( unblocked->units.head->data->type == TYPE_PROCESSING ) {
      insertIntoReadyState( unblocked, ready, scheduler );
    }
    else {
      printf( "Error: unknown execution unit type %d\n", unblocked->units.head->data->type );
      exit( 1 );
    }
  }
}

void doContextSwitch( struct ProcessList * ready,
		      struct ProcessList * blocked,
		      struct ProcessList * waiting,
		      long * tick,
		      long * contextSwitchTicks,
		      int scheduler ) {
  int i;
  for( i = 0; i < CONTEXT_SWITCH_TIME; i++ ) {
                                        // While we are performing a context
                                        //   switch, blocked processes and
                                        //   those not yet started advance,
                                        //   while ready ones continue to wait.
    updateProcessesInList( waiting );
    updateProcessesInList( blocked );
                                        
                                        // Some may finish their execution unit
    moveFinishedBlockedProcesses( blocked,
				  ready,
				  *tick,
				  scheduler );
    moveFinishedWaitingProcesses( waiting,
				  ready,
				  *tick,
				  scheduler );
    (*tick)++;
    (*contextSwitchTicks)++;
  }
}
