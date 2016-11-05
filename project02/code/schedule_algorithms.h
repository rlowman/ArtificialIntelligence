// File: schedule_algorithms.h
// Author: Chad Hogg
// Version: 2014-10-22
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It declares functions for each scheduling algorithm supported.

//// The different scheduler options:

#define SCHEDULER_ROUND_ROBIN 1
// TODO: Continue creating these for each scheduling algorithm added, each 
//   getting a unique non-zero value.

#define SCHEDULER_FIRST_COME_FIRST_SERVED 2


//// The actual scheduling algorithms:

/**
 * Choose a process to run next, using the Preemptive Round Robin algorithm.
 *
 * @param running A pointer to the currently-running process (could be NULL).
 * @param ready A pointer to the list of ready processes.
 * @param blocked A pointer to the list of blocked processes.
 * @param waiting A pointer to the list of waiting processes.
 * @param tick A pointer to the current simulation time counter.
 * @param contextSwitchTicks A pointer to the number of ticks wasted in context switches.
 * @param quantum The length of the quantum before we preempt.
 * @return A pointer to the process that should run next (could be NULL).
 **/
struct Process * schedulePreemptiveRoundRobin( struct Process * running,
					       struct ProcessList * ready,
					       struct ProcessList * blocked,
					       struct ProcessList * waiting,
					       long * tick,
					       long * contextSwitchTicks,
					       int quantum );

struct Process * scheduleNonpreemptiveFirstComeFirstServed( struct Process * running,
					       struct ProcessList * ready,
					       struct ProcessList * blocked,
					       struct ProcessList * waiting,
					       long * tick,
					       long * contextSwitchTicks);
