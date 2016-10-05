// File: project01/main.c
// Author: Robert Lowman
// Implementation of the check-sum server for project01

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <proj01-networking.h>

// The port number of the port we are requesting information from.
int port;

/**
 * Sets the new worker thread count when requested by the user.
 * 
 * @return the new amount of worker threads to use.
 **/
int selectThreadCount() {
  int * returnValue;
  printf("Enter the amount of worker threads you wish to use: ");
  scanf("%d", &returnValue);
  return returnValue;
}

/**
 * Sets the new port number when requested by the user.
 *
 * @return the new port number to use.
 **/
int selectPort() {
  int * returnValue;
  printf("Enter the port number you wish to use: ");
  scanf("%d", &returnValue);
  return returnValue;
}

/**
 * 
 **/
void * initialize_to_server(void * index) {
  int listenSocket = server_initialize_networking(port);

  while(true) {
    
  }
}

int main(int argc, char * argv[]) {
  int threadDefault = 4;
  if(argc == 2) {
    if(strcmp(argv[1], "-p") == 0) {
      port = selectPort();
    }
    else if(strcmp(argv[1], "-t") == 0) {
      threadDefault = selectThreadCount;
    }
  }
  else if(argc == 3) {
    if(strcmp(argv[1], "-p") == 0 ||
       strcmp(argv[2], "-p") == 0) {
      port = selectPort();
    }
    if(strcmp(argv[1], "-t") == 0 ||
       strcmp(argv[2], "-t") == 0) {
      threadDefault = selectThreadCount;
    }
  }

  pthread_t dispatcherThread;
  pthread_t workerThreads[threadDefault];
  int count = 0;

  int status = pthread_create(&dispatcherThread, NULL,
			      initialize_to_server, (void *)i);

  if(status != 0) {
    printf("Error %d creating dispather thread. \n", status);
    exit(1);
  }
  long i;
  for(i = 0; i < threadDeafult; i++) {
    
  }
  return 0;
}
