// File: process.h
// Author: Chad Hogg
// Version: 2014-10-21
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It contains declarations of types to repesent a process and functions for
//   interacting with processes.

/**
 * The number of characters in a process name.
 **/
#define NAME_SIZE 64

/**
 * A representation of a process in the midst of execution.
 **/
struct Process {

  /**
   * The name of this process, used only for reporting.
   **/
  char name[NAME_SIZE];

  /**
   * A list of execution units that must be completed for this process.
   **/
  struct ExecutionUnitList units;

  /**
   * How important this process is: 1-100, higher is more important.
   **/
  int priority;

  /**
   * What time this process was created.
   **/
  long startTime;

  /**
   * What time this process terminated.
   **/
  long finishTime;

  /**
   * When the process most recently finished blocking for an interactive device.
   **/
  long lastInteractiveBlockEnd;

  /**
   * The total time this process has been waiting between interactive blocks.
   **/
  long totalInteractiveWaitTime;

  /**
   * The number of times this process has waited in between two interactive blocks.
   **/
  long numberOfInteractiveWaits;
};

/**
 * Create a process of type A, which is an interactive process like an editor.
 *
 * @param name The name of this process.
 * @param priority The priority of the process you are creating.
 * @param delay How many milliseconds before the process is created.
 * @return A pointer to a process, which should be freed when not needed.
 **/
struct Process * createProcTypeA( char name[NAME_SIZE], 
				  int priority,
				  int delay );


/**
 * Create a process of type B, which is some kind of server.
 *
 * @param name The name of this process.
 * @param priority The priority of the process you are creating.
 * @param delay How many milliseconds before the process is created.
 * @return A pointer to a process, which should be freed when not needed.
 **/
struct Process * createProcTypeB( char name[NAME_SIZE],
				  int priority,
				  int delay );


/**
 * Create a process of type C, which is something like a compiler.
 *
 * @param name The name of this process.
 * @param priority The priority of the process you are creating.
 * @param delay How many milliseconds before the process is created.
 * @return A pointer to a process, which should be freed when not needed.
 **/
struct Process * createProcTypeC( char name[NAME_SIZE],
				  int priority,
				  int delay );

/**
 * Create a process of type D, which is some kind of small background job.
 *
 * @param name The name of this process.
 * @param priority The priority of the process you are creating.
 * @param delay How many milliseconds before the process is created.
 * @return A pointer to a process, which should be freed when not needed.
 **/
struct Process * createProcTypeD( char name[NAME_SIZE],
				  int priority,
				  int delay );
