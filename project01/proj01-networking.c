// File: proj01-networking.c
// Author: Chad Hogg
// Last Modified: 2014-09-24
// Implementations of a few functions for networking.
// Provided as part of project 01 in CS 364 at King's College in Spring 2013.

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include "proj01-networking.h"

int server_initialize_networking( int port ) {
  int listen_socket;                    // The descriptor for a socket that
                                        //   will be used to get requests for
                                        //   connections to this process.
  struct sockaddr_in my_addr;           // The address of this process.
  int ret;                              // The return value from various system
                                        //   calls.

                                        // Create a socket that processes will
                                        //   use to request connections to me.
  listen_socket = socket( AF_INET,      // Use the Internet (IPv4) protocol.
			  SOCK_STREAM,  // Use the TCP protocol.
			  0 );          // No flags needed.
  if( listen_socket < 0 ) {
    perror( "Error: opening listening socket" );
    exit( 1 );
  }

  my_addr.sin_family = AF_INET;         // I have an IPv4 address.
  my_addr.sin_addr.s_addr = INADDR_ANY; // Automatically get my address.
  my_addr.sin_port = htons( port );     // Use the specified port.

                                        // Bind the socket I created to this
                                        //   address.
  ret = bind( listen_socket,            // The socket.
                                        // My address.
	      (struct sockaddr *)&my_addr,
	      sizeof( my_addr ) );      // Storage for my address's size.
  if( ret != 0 ) {
    perror( "Error: binding to listening socket" );
    exit( 1 );
  }

                                        // Put this socket in listening mode.
  ret = listen( listen_socket, MAX_WAITING_CONNECTIONS );
  if( ret != 0 ) {
    perror( "Error: listening with listening socket" );
    exit( 1 );
  }

  return listen_socket;
}

int server_get_connection( int listen_socket ) {
  struct sockaddr_in client_addr;       // The address of the process that
                                        //   wants to connect to me.
  socklen_t address_len;                // The size of an address structure.
                                        //   I need an actual variable so a
                                        //   system call can put a value there.
  int new_socket;                       // The ultimate goal of this function.

  address_len = sizeof( client_addr );  // Get the size of an address.
                                        // Accept a request for a connection,
                                        //   blocking until we receive one.
  new_socket = accept( listen_socket,   // The socket I am listening on.
                                        // A place to put the address of the
                                        //   process that requests the
                                        //   connection.
		   (struct sockaddr *)&client_addr, 
		   &address_len );      // A place to put the size of address.
  if( new_socket < 0 ) {
    perror( "Error: accepting a socket" );
    exit( 1 );
  }

  return new_socket;                    // This is now the socket (just a
                                        //   special type of file descriptor)
                                        //   with which I can communicate with
                                        //   the other process.
}

int client_connect( const char * ip_addr, int port ) {
  int the_socket;                       // What we are looking for.
  int ret;                              // The return value from various system
                                        //   calls.
  struct sockaddr_in server_address;    // The address of the server process to
                                        //   which I would like to connect.

                                        // Create a socket with which we will
                                        //   communicate with the server.
  the_socket = socket( AF_INET,         // Use the Internet (IPv4) protocol.
		       SOCK_STREAM,     // Use the TCP protocol.
		       0 );             // No flags needed.
  if( the_socket < 0 ) {
    perror( "Error: opening socket" );
    exit( 1 );
  }

  server_address.sin_family = AF_INET;  // The server has an IPv4 address.
                                        // Use the specified IPv4 address.
  ret = inet_aton( ip_addr, &server_address.sin_addr );
  if( ret == 0 ) {
    perror( "Error: invalid address" );
  }
                                        // Use the specified port.
  server_address.sin_port = htons( port );

                                        // Create a connection to the server.
  ret = connect( the_socket,            // The socket we are using.
                                        // The server's address.
		 (struct sockaddr *)&server_address,
		                        // The size of the server's address.
		 sizeof( server_address ) );

  if( ret != 0 ) {
    if( errno == EADDRNOTAVAIL ) {
      printf( "You have encountered an error that only seems to occur with very large numbers of requests being processed very quickly, and that Dr. Hogg has not managed to debug.  Let him know that you saw this, then try using a smaller number of requests.\n" );
      exit( 1 );
    }
    else {
      perror( "Error: connecting with socket" );
      exit( 1 );
    }
  }

  return the_socket;
}
