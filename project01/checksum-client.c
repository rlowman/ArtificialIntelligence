// File: checksum-client.c
// Author: Chad Hogg
// Last Modified: 2014-09-24
// A client that will request lots of checksums from a server process.
// Provided as part of project 01 in CS 364 at King's College in Fall 2014.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "proj01-networking.h"

#define BUFFER_SIZE (1024 > PATH_MAX + 50 ? 1024 : PATH_MAX + 50)

#define DEFAULT_NUM_WORK 50             // How many requests to make if not
                                        //   overridden by the user.

#define STATUS_NOT_STARTED 0            // We have not sent this request yet.
#define STATUS_PENDING 1                // We sent the request and are waiting
                                        //   for a response.
#define STATUS_FINISHED 2               // We have received the response from
                                        //   the server.
#define STATUS_ERROR 3                  // We were unable to get a response
                                        //   from the server due to a network
                                        //   error.  This should never happen,
                                        //   but it does, and I cannot figure
                                        //   out why.

int verbosity;                          // How much information to print.

/**
 * A request sent to the server.
 **/
struct work {
  int socket_fd;                        // The socket the request was sent on.
  int request_num;                      // A serial number.
  char filename[PATH_MAX];              // The file whose checksum we want.
  long correct_answer;                  // What I know the checksum to be.
  long received_answer;                 // The checksum from the server.
  int status;                           // The status of this request.        
};

int num_work;                           // The total number of requests.
struct work * jobs;                     // An array of requests, dynamically
                                        //   allocated in main.

int num_pending;                        // The number of jobs that are
                                        //   currently pending.
pthread_mutex_t pending_mutex;          // A mutex to protect num_pending.

/**
 * The entry point for worker threads that wait for responses from the server.
 * Note: there will be a lot of these threads, and they should be detached.
 *
 * @param thread_id The request number this thread handles (cast to long).
 * @return Unused, so always NULL.
 **/
void * receiver( void * thread_id ) {
  char buffer[BUFFER_SIZE];             // The data we read from the server.
  char error_buf[BUFFER_SIZE];          // Error messages.
  char filename[PATH_MAX];              // The filename part of the message.
  int ret;                              // Various return values.
  char * file_start, * file_end;        // Pointers for breaking up the message
                                        //   into their three components.

                                        // The job should always be pending
                                        //   before this thread starts and stay
                                        //   that way until this thread is
                                        //   nearly finished.
  if( jobs[(long)thread_id].status != STATUS_PENDING ) {
    printf( "Error: job had bad status at start for thread %ld.\n", (long)thread_id );
    exit( 3 );
  }

                                        // Read the server's response, blocking
                                        //   until it exists if necessary.
  ret = read( jobs[(long)thread_id].socket_fd, 
	      buffer, 
	      BUFFER_SIZE - 1 );
  if( ret < 0 ) {
    snprintf( error_buf, BUFFER_SIZE, "Error: reading from socket for request %d", jobs[(long)thread_id].request_num );
    perror( error_buf );
    jobs[(long)thread_id].status = STATUS_ERROR;
    exit( 3 );
  }
  buffer[ret] = '\0';                   // Null-terminate the string.

  ret = close( jobs[(long)thread_id].socket_fd );
  if( ret != 0 ) {
    snprintf( error_buf, BUFFER_SIZE, "Error: closing socket for request %d", jobs[(long)thread_id].request_num );
    perror( error_buf );
    exit( 3 );
  }

                                        // The first part of the string 
                                        //   contains the serial number.
  jobs[(long)thread_id].request_num = atoi( buffer );
                                        // Then we have the filename, which
                                        //   should match.
  file_start = strchr( buffer, ' ' ) + 1;
  file_end = strchr( file_start, ' ' );
  strncpy( filename, 
	   file_start, 
	   file_end - file_start );
  filename[file_end - file_start] = '\0';
  if( strcmp( jobs[(long)thread_id].filename, filename ) != 0 ) {
    printf( "Error: expected filename \"%s\", but got back filename \"%s\".\n", jobs[(long)thread_id].filename, filename );
    printf( "Full buffer was \"%s\".\n", buffer );
    exit( 3 );
  }
                                        // Then we have the checksum.
  jobs[(long)thread_id].received_answer = atol( file_end );
                                        // This job is now finished.
  jobs[(long)thread_id].status = STATUS_FINISHED;
  pthread_mutex_lock( &pending_mutex );
  num_pending--;                        // This job is no longer pending.
  pthread_mutex_unlock( &pending_mutex );
  return NULL;                          // No one cares about this.
}

/**
 * The entry point for a thread that prints a status update every second.
 *
 * @param unused Unused.
 * @return Unused (and therefore always NULL).
 **/
void * updater( void * unused ) {
  int update_num = 0;                   // Which update is this.
  int num_not_started = num_work,       // How many requests not started.
    num_success = 0,                    // How many succeeded.
    num_failure = 0,                    // How many with wrong value.
    num_error = 0;                      // How many died with errors.
  int i;

                                        // Continue until everything is
                                        //   finished or error.
  while( num_success + num_failure + num_error != num_work ) {
    if( verbosity > 0 ) {
      printf( "Update Number %d:\n", update_num );
    }
    update_num++;
    num_not_started = 0;
    num_success = 0;
    num_failure = 0;
    num_error = 0;

    if( verbosity > 0 ) {
      printf( "\tPending: " );
    }
    for( i = 0; i < num_work; i++ ) {
      if( jobs[i].status == STATUS_NOT_STARTED ) {
	num_not_started++;
      }
      else if( jobs[i].status == STATUS_PENDING ) {
	if( verbosity > 0 ) {
	  printf( "%d ", jobs[i].request_num );
	}
      }
      else if( jobs[i].status == STATUS_FINISHED ) {
	if( jobs[i].correct_answer == jobs[i].received_answer ) {
	  num_success++;
	}
	else {
	  num_failure++;
	}
      }
      else if( jobs[i].status == STATUS_ERROR ) {
	num_error++;
      }
    }

    if( verbosity > 0 ) {
      printf( "\n" );
      printf( "\tNumber not yet started: %d\n", num_not_started );
      printf( "\tNumber correct answers: %d\n", num_success );
      printf( "\tNumber wrong answers: %d\n", num_failure );
      printf( "\tNetwork errors: %d\n", num_error );
    }
    sleep( 1 );                         // Do nothing for approximately a
                                        //   second, then do it all again.
  }

  return NULL;                          // No one cares about this.
}

/**
 * Compute a (weak) checksum for a specified file.
 *
 * @param filename The full path to a file on disk.
 * @return The sum of the bytes in that file.
 **/
long compute_checksum( char * filename ) {
  int fd;
  char error_buf[BUFFER_SIZE];
  char buffer[BUFFER_SIZE];
  int numRead, i;
  long result = 0L;

  fd = open( filename,
	     O_RDONLY );
  if( fd < 0 ) {
    snprintf( error_buf, BUFFER_SIZE, "Error: opening file %s", filename );
    perror( error_buf );
    exit( 2 );
  }

  numRead = read( fd, 
		  buffer, 
		  BUFFER_SIZE );
  while( numRead > 0 ) {
    for( i = 0; i < numRead; i++ ) {
      result = result + (long)buffer[i];
    }
    numRead = read( fd, 
		    buffer, 
		    BUFFER_SIZE );
  }
  if( numRead < 0 ) {
    snprintf( error_buf, BUFFER_SIZE, "Error: reading file %s", filename );
    perror( error_buf );
    exit( 2 );
  }
  numRead = close( fd );
  if( numRead != 0 ) {
    snprintf( error_buf, BUFFER_SIZE, "Error: closing file %s", filename );
    perror( error_buf );
    exit( 2 );
  }
  return result;
}

void initialize_jobs() {
  int i;
                                        // Allocate the array of requests to
                                        //   make and fill it with un-started
                                        //   ones.
  jobs = malloc( num_work * sizeof( struct work ) );
  for( i = 0; i < num_work; i++ ) {
    jobs[i].request_num = i;
    jobs[i].status = STATUS_NOT_STARTED;
    switch( i % 5 ) {
    case 0:
      strncpy( jobs[i].filename, "/usr/include/elf.h", PATH_MAX );
      jobs[i].correct_answer = compute_checksum( "/usr/include/elf.h" );
      break;
    case 1:
      strncpy( jobs[i].filename, "/usr/include/time.h", PATH_MAX );
      jobs[i].correct_answer = compute_checksum( "/usr/include/time.h" );
      break;
    case 2:
      strncpy( jobs[i].filename, "/usr/include/string.h", PATH_MAX );
      jobs[i].correct_answer = compute_checksum( "/usr/include/string.h" );
      break;
    case 3:
      strncpy( jobs[i].filename, "/usr/include/limits.h", PATH_MAX );
      jobs[i].correct_answer = compute_checksum( "/usr/include/limits.h" );
      break;
    case 4:
      strncpy( jobs[i].filename, "/usr/include/stdio.h", PATH_MAX );
      jobs[i].correct_answer = compute_checksum( "/usr/include/stdio.h" );
      break;
    }
  }
}

int main( int argc, char * argv[] ) {
  int port = DEFAULT_PORT;              // What port we expect the server to be
                                        //   listening on.
  const char * ip_addr = DEFAULT_IP;    // Where the server is located.
  int i, ret;
  char buffer[BUFFER_SIZE];
  pthread_attr_t attributes;            // Attributes for the worker threads.
  pthread_t worker_id, updater_id;
  time_t start, end;
  int num_written;

  num_work = DEFAULT_NUM_WORK;
  num_pending = 0;
  verbosity = 0;

  ret = pthread_attr_init( &attributes );
  if( ret != 0 ) {
    perror( "Error: initializing thread attributes" );
    exit( 2 );
  }

                                        // Worker threads should be detached
                                        //   because I do not want to keep
                                        //   track of them individually.
  ret = pthread_attr_setdetachstate( &attributes, 
				     PTHREAD_CREATE_DETACHED );
  if( ret != 0 ) {
    perror( "Error: setting thread attribute to detached" );
    exit( 2 );
  }

  ret = pthread_mutex_init( &pending_mutex,
			    NULL );
  if( ret != 0 ) {
    perror( "Error: initializing mutex" );
    exit( 2 );
  }
  
  for( i = 1; i < argc; i++ ) {
    if( strcmp( argv[i], "--help" ) == 0 ) {
      printf( "Usage: checksum-client [OPTION] ...\n" );
      printf( "Send requests for file checksums to a server, verify them, and time it.\n" );
      printf( "\nOptions:\n" );
      printf( "  -i ZZZ\t Expect the server to be at IP address ZZZ, which should be in dotted decimal notation.  Default: %s.\n", DEFAULT_IP );
      printf( "  -p XXX\tExpect the server to be listening on port XXX, which must be in the range [1024,49151].  Default: %d.\n", DEFAULT_PORT );
      printf( "  -w YYY\tGenerate YYY requests.  Default: %d.\n", DEFAULT_NUM_WORK );
      printf( "  -v\tBe verbose in output.\n" );
      printf( "  -vv\tBe very verbose in output.\n" );
      printf( "  --help\tDisplay this message and exit.\n" );
      exit( 0 );
    }
    else if( strcmp( argv[i], "-i" ) == 0 ) {
      if( argc > i + 1 ) {
	ip_addr = argv[i+1];
	i++;
      }
      else {
	printf( "Error: the -i argument must be followed by an IP address.\n" );
	exit( 1 );
      }
    }
    else if( strcmp( argv[i], "-p" ) == 0 ) {
      if( argc > i + 1 ) {
	port = atoi( argv[i+1] );
	i++;
	if( port < 1024 || port > 49151 ) {
	  printf( "Error: following -p must be a number in the range [1024,49151].\n" );
	  exit( 1 );
	}
      }
      else {
	printf( "Error: the -p argument must be followed by a port number.\n" );
	exit( 1 );
      }
    }
    else if( strcmp( argv[i], "-w" ) == 0 ) {
      if( argc > i + 1 ) {
	num_work = atoi( argv[i+1] );
	i++;
	if( num_work < 1 ) {
	  printf( "Error: following -w must be a positive number.\n" );
	  exit( 1 );
	}
      }
      else {
	printf( "Error: the -w argument must be followed by a port number.\n" );
	exit( 1 );
      }
    }
    else if( strcmp( argv[i], "-v" ) == 0 ) {
      verbosity = (verbosity > 1 ? verbosity : 1);
    }
    else if( strcmp( argv[i], "-vv" ) == 0 ) {
      verbosity = 2;
    }
    else {
      printf( "Error: unknown argument \"%s\".\n", argv[i] );
      exit( 1 );
    }
  }

  initialize_jobs();

                                        // Create the thread that will update
                                        //   the UI approximately every second.
  ret = pthread_create( &updater_id,
			NULL, 
			updater, 
			NULL );
  if( ret != 0 ) {
    perror( "Error: creating the updater thread" );
    exit( 2 );
  }

  start = time( NULL );                 // Record when we started requesting.
  for( i = 0; i < num_work; i++ ) {
                                        // Limit the number of simultaneous
                                        //   connections.  Otherwise some get
                                        //   dropped.
    while( num_pending > MAX_WAITING_CONNECTIONS ) {
      sched_yield();
    }
                                        // Connect to the server to make a 
                                        //   request.
    jobs[i].socket_fd = client_connect( ip_addr, port );
    snprintf( buffer, BUFFER_SIZE, "%d %s", jobs[i].request_num, jobs[i].filename );
                                        // Actually make the request.
    num_written = write( jobs[i].socket_fd, 
			 buffer, 
			 strlen( buffer ) );

    if( verbosity > 1 ) {
      struct sockaddr_in addr;
      socklen_t addrlen = sizeof(addr);
      getsockname(jobs[i].socket_fd, (struct sockaddr *)&addr, &addrlen);
      printf("Wrote job %d request through port %d.\n", jobs[i].request_num, ntohs(addr.sin_port));
    }

    if( num_written != strlen( buffer ) ) {
      perror( "Error: writing to socket" );
      exit( 3 );
    }

    jobs[i].status = STATUS_PENDING;    // We are now waiting for the server.
                                        // Create a worker thread to wait for
                                        //   the server to respond and verify
                                        //   their answer.
    ret = pthread_mutex_lock( &pending_mutex );
    if( ret != 0 ) {
      perror( "Error: locking mutex" );
      exit( 3 );
    }
    num_pending++;
    ret = pthread_mutex_unlock( &pending_mutex );
    if( ret != 0 ) {
      perror( "Error: unlocing mutex" );
      exit( 3 );
    }
    ret = pthread_create( &worker_id, 
			  &attributes, 
			  receiver, 
			  (void *)(long)i );
    if( ret != 0 ) {
      perror( "Error: creating worker thread" );
      exit( 3 );
    }
  }

  end = time( NULL );                   // Record when we stopped.

                                        // Wait for the updater, which exits
                                        //   when every request is finished or
                                        //   dead from a network error.
  ret = pthread_join( updater_id, 
		      NULL );
  if( ret != 0 ) {
    perror( "Error: joining updater thread" );
    exit( 4 );
  }

                                        // Report how long this took.
  printf( "Total time to process %d requests was %lds.\n", num_work, (long)difftime( end, start ) );

                                        // Clean up resources.
  ret = pthread_mutex_destroy( &pending_mutex );
  if( ret != 0 ) {
    perror( "Error: destroying mutex" );
    exit( 4 );
  }
  ret = pthread_attr_destroy( &attributes );
  if( ret != 0 ) {
    perror( "Error: destroying thread attributes" );
    exit( 4 );
  }
  free( jobs );

  return 0;
}
