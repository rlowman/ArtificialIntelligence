// File: proj01-networking.h
// Author: Chad Hogg
// Last Modified: 2014-09-24
// Declares a few functions for doing networking.
// Provided as part of project 01 in CS 364 at King's College in Fall 2014.

/**
 * The IP address for local connections to the same machine.
 **/
#define DEFAULT_IP "127.0.0.1"

/**
 * A "well-known" port number that may be used by default if none is specified.
 * Chosen because it is not actually well-known for any protocol.
 **/
#define DEFAULT_PORT 5006

/**
 * The number of socket connections that may be waiting to be accepted.
 **/
#define MAX_WAITING_CONNECTIONS 128

/**
 * Prepare to listen for networking connection requests from other processes.
 * This function should usually only be called once at the beginning of your
 *   (server) program.
 *
 * @param port Our "well-known" port number on which we should listen.
 *   This should be a number in the range [1024,49151].
 * @return A file descriptor that represents a socket on which we are listening
 *   for network connection requests.
 **/
int server_initialize_networking( int port );

/**
 * Accept a connection request from some other process.
 * This should be called many times (probably an infinite number).
 *
 * @param listen_socket A file descriptor that was previously returned from the
 *   server_initialize_networking function.
 * @return A file descriptor that represents a socket through which we can
 *   communicate with a specific client, reading a request and writing a 
 *   response.
 **/
int server_get_connection( int listen_socket );

/**
 * Connect to a server process.
 *
 * @param ip_addr The server's IP(v4) address in dotted decimal notation.
 * @param port The "well-known" port on which the server is listening for us.
 * @return A file descriptor that represents a socket through which we can
 *   communicate with the server.
 **/
int client_connect( const char * ip_addr, int port );
