// File: execution_unit.h
// Author: Chad Hogg
// Version: 2014-10-21
// This file is part of project 02 in CS364 at King's College in Fall 2014.
// It contains declarations of types for execution units, which are parts of
//   what a process needs to do to complete.

//// C does not have enumerations, so C programmers do this sort of thing
////   to declare a number of macros to represent different possibilities.
//// These macros represent the different types of execution units.

/**
 * This execution unit involves using the CPU.
 **/
#define TYPE_PROCESSING 0

/**
 * This execution unit involves waiting for some non-interactive device.
 **/
#define TYPE_BLOCKED 1

/**
 * This execution unit involves waiting for an interactive device.
 **/
#define TYPE_INTERACTIVE 2

/**
 * This execution unit represents waiting until the process is created.
 **/
#define TYPE_NOT_YET_STARTED 3


/**
 * A part of what needs to be done in order to complete a process.
 **/
struct ExecutionUnit {

  /**
   * The number of milliseconds remaining in this execution unit.
   **/
  int mSecsLeft;

  /**
   * The type of this execution unit.
   * Should be either TYPE_PROCESSING, TYPE_BLOCKED, TYPE_INTERACTIVE, or TYPE_NOT_YET_STARTED.
   **/
  int type;
};
