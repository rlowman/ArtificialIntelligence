// File: schedule_algorithms.c
// Author: Chad Hogg
// Version: 2014-10-22
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It defines functions for each scheduling algorithm supported.

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "execution_unit.h"
#include "execution_unit_list.h"
#include "process.h"
#include "process_list.h"
#include "schedule_utilities.h"
#include "schedule_algorithms.h"


struct Process * schedulePreemptiveRoundRobin( struct Process * running,
					       struct ProcessList * ready,
					       struct ProcessList * blocked,
					       struct ProcessList * waiting,
					       long * tick,
					       long * contextSwitchTicks,
					       int quantum ) {
  static int sinceLastSwitch = 0;       // Count how many milliseconds have
                                        //   passed since the last context
                                        //   switch, so that we know when to do
                                        //   a preemption.
                                        // This is static so that it is
                                        //   preserved from one function call
                                        //   to another.

  if( running != NULL &&                // Preempt the running process if there
      sinceLastSwitch >= quantum &&     //   is one and it has run for a full
      ready->head != NULL ) {           //   quantum and there are others that
                                        //   would like to run.
    enqueueProcess( ready, running );
    running = NULL;
  }

  if( running == NULL &&                // Choose a new process if none is
      ready->head != NULL ) {           //   running and some are ready.
    doContextSwitch( ready,
		     blocked,
		     waiting,
		     tick,
		     contextSwitchTicks,
		     SCHEDULER_ROUND_ROBIN );
                                        // Get the process that has been
                                        //   sitting in the ready queue the
                                        //   longest.
    running = dequeueProcess( ready );
    sinceLastSwitch = 0;
  }
  sinceLastSwitch++;                    // One more millisecond since we last
                                        //   did a context switch.

  return running;                       // Return what the running process
                                        //   should be, which will not have
                                        //   changed unless there had been none
                                        //   or we preempted.
}

struct Process * scheduleNonpreemptiveFirstComeFirstServed(struct Process * running,
							   struct ProcessList * ready,
							   struct ProcessList * blocked,
							   struct ProcessList * waiting,
							   long * tick,
							   long * contextSwitchTicks) {      

  if( running == NULL &&                
      ready->head != NULL ) {
   
    doContextSwitch( ready,
		     blocked,
		     waiting,
		     tick,
		     contextSwitchTicks,
		     SCHEDULER_FIRST_COME_FIRST_SERVED );
                                       
    running = dequeueProcess( ready );
  }                 

  return running;                      
}

struct Process * scheduleNonpreemptiveShortestJobFirst(struct Process * running,
						       struct ProcessList * ready,
						       struct ProcessList * blocked,
						       struct ProcessList * waiting,
						       long * tick,
						       long * contextSwitchTicks) {      

  if( running == NULL &&                
      ready->head != NULL ) {
    printf("before switch\n");
    doContextSwitch( ready,
		     blocked,
		     waiting,
		     tick,
		     contextSwitchTicks,
		     SCHEDULER_NONPREEMPTIVE_SHORTEST_JOB );
    printf("after\n");
                                       
    running = dequeueProcessSJF( ready );
  }                 

  return running;                      
}

struct Process * schedulePreemptiveHighestPriority(struct Process * running,
						   struct ProcessList * ready,
						   struct ProcessList * blocked,
						   struct ProcessList * waiting,
						   long * tick,
						   long * contextSwitchTicks,
						   int quantum) {
  static int sinceLastSwitch = 0;       
  if( running != NULL &&                
      sinceLastSwitch >= quantum &&     
      ready->head != NULL ) {           
    enqueueProcess( ready, running );
    running = NULL;
  }

  if( running == NULL &&               
      ready->head != NULL ) {           
    doContextSwitch( ready,
		     blocked,
		     waiting,
		     tick,
		     contextSwitchTicks,
		     SCHEDULER_PREEMPTIVE_HIGHEST_PRIORITY );
                                    
    running = dequeueProcessHP( ready );
    sinceLastSwitch = 0;
  }
  sinceLastSwitch++;                    
  return running;                       
}

struct Process * schedulePreemptiveMultipleQueues(struct Process * running,
						   struct ProcessList * ready,
						   struct ProcessList * blocked,
						   struct ProcessList * waiting,
						   long * tick,
						   long * contextSwitchTicks) {
  static int sinceLastSwitch = 0;
  int quantum = 5;
  if( running != NULL &&                
      sinceLastSwitch >= quantum &&     
      ready->head != NULL ) {           
    enqueueProcess( ready, running );
    running = NULL;
  }

  if( running == NULL &&               
      ready->head != NULL ) {           
    doContextSwitch( ready,
		     blocked,
		     waiting,
		     tick,
		     contextSwitchTicks,
		     SCHEDULER_MULTIPLE_QUEUES );
                                    
    running = dequeueProcessMQ( ready );
    sinceLastSwitch = 0;
  }
  sinceLastSwitch++;                    
  return running;                       
}

struct Process * schedulePreemptiveLongestJobFirst( struct Process * running,
					       struct ProcessList * ready,
					       struct ProcessList * blocked,
					       struct ProcessList * waiting,
					       long * tick,
					       long * contextSwitchTicks,
					       int quantum ) {
  static int sinceLastSwitch = 0;       
  if( running != NULL &&                
      sinceLastSwitch >= quantum &&     
      ready->head != NULL ) {           
    enqueueProcess( ready, running );
    running = NULL;
  }

  if( running == NULL &&               
      ready->head != NULL ) {           
    doContextSwitch( ready,
		     blocked,
		     waiting,
		     tick,
		     contextSwitchTicks,
		     SCHEDULER_PREEMPTIVE_LONGEST_JOB );
                                    
    running = dequeueProcessLJF( ready );
    sinceLastSwitch = 0;
  }
  sinceLastSwitch++;                    
  return running;  
}  
