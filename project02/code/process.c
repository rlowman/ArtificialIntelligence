// File: process.c
// Author: Chad Hogg
// Version: 2014-10-21
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It contains definitions of functions for interacting with processes.

#include <stdlib.h>
#include <string.h>

#include "execution_unit.h"
#include "execution_unit_list.h"
#include "process.h"

struct Process * createProcTypeA( char name[NAME_SIZE],
				  int priority,
				  int delay )
{
  struct Process * creation = malloc( sizeof( struct Process ) );
  creation->units.head = NULL;
  creation->units.tail = NULL;
  strncpy( creation->name, name, NAME_SIZE );
  creation->startTime = 0;
  creation->priority = priority;
  creation->totalInteractiveWaitTime = 0;
  creation->numberOfInteractiveWaits = 0;

  if( delay != 0 ) {
    enqueueExecutionUnit( &creation->units, delay, TYPE_NOT_YET_STARTED );
  }
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 20, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 200, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 20, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 200, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 20, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 200, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 20, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 200, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 20, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 200, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 20, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 200, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 20, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 10, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  return creation;
}

struct Process * createProcTypeB( char name[NAME_SIZE],
				  int priority,
				  int delay )
{
  struct Process * creation = malloc( sizeof( struct Process ) );
  creation->units.head = NULL;
  creation->units.tail = NULL;
  strncpy( creation->name, name, NAME_SIZE );
  creation->startTime = 0;
  creation->priority = priority;
  creation->totalInteractiveWaitTime = 0;
  creation->numberOfInteractiveWaits = 0;

  if( delay != 0 ) {
    enqueueExecutionUnit( &creation->units, delay, TYPE_NOT_YET_STARTED );
  }
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 100, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 100, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 100, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 100, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 100, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 100, TYPE_INTERACTIVE );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  return creation;
}

struct Process * createProcTypeC( char name[NAME_SIZE],
				  int priority,
				  int delay )
{
  struct Process * creation = malloc( sizeof( struct Process ) );
  creation->units.head = NULL;
  creation->units.tail = NULL;
  strncpy( creation->name, name, NAME_SIZE );
  creation->startTime = 0;
  creation->priority = priority;
  creation->totalInteractiveWaitTime = 0;
  creation->numberOfInteractiveWaits = 0;

  if( delay != 0 ) {
    enqueueExecutionUnit( &creation->units, delay, TYPE_NOT_YET_STARTED );
  }
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 5000, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 10, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  return creation;
}

struct Process * createProcTypeD( char name[NAME_SIZE],
				  int priority,
				  int delay )
{
  struct Process * creation = malloc( sizeof( struct Process ) );
  creation->units.head = NULL;
  creation->units.tail = NULL;
  strncpy( creation->name, name, NAME_SIZE );
  creation->startTime = 0;
  creation->priority = priority;
  creation->totalInteractiveWaitTime = 0;
  creation->numberOfInteractiveWaits = 0;

  if( delay != 0 ) {
    enqueueExecutionUnit( &creation->units, delay, TYPE_NOT_YET_STARTED );
  }
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 20, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 100, TYPE_PROCESSING );
  enqueueExecutionUnit( &creation->units, 10, TYPE_BLOCKED );
  enqueueExecutionUnit( &creation->units, 1, TYPE_PROCESSING );
  return creation;
}
