// File: project01/main.c
// Author: Robert Lowman
// Implementation of the check-sum server for project01

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "proj01-networking.h"

// The port number of the port we are requesting information from.
int port = DEFAULT_PORT;

pthread_mutex_t mutex;

pthread_cond_t openSpace;

pthread_cond_t popSpace;

int workQueue[8];

volatile int nextPush;

volatile int nextPop;

volatile int filled;

/**
 * Sets the new worker thread count when requested by the user.
 * 
 * @return the new amount of worker threads to use.
 **/
int selectThreadCount() {
  int returnValue;
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
  int returnValue;
  printf("Enter the port number you wish to use: ");
  scanf("%d", &returnValue);
  return returnValue;
}

/**
 * Connects the dispatcher thread to the server
 * 
 * @param foo Unused.
 * @return Unused.
 **/
void * initialize_to_server(void * foo) {
  int listen_socket = server_initialize_networking(port);
  int currentFileDescriptor;
  void * returnValue;
  while(1) {

    currentFileDescriptor = server_get_connection(listen_socket);
    pthread_mutex_lock(&mutex);
    workQueue[nextPush] = currentFileDescriptor;
    nextPush = (nextPush + 1) % 8;
    filled ++;
    
    if(filled != 8) {
      pthread_cond_broadcast(&openSpace);
    }
    else {
      pthread_cond_wait(&openSpace, &mutex);
    }
    pthread_mutex_unlock(&mutex);
  }
  return returnValue;
}

/**
 * Implements the workers thread functionality
 *
 * @param foo Unused.
 * @return Unused.
 **/
void * initialize_worker_thread(void * index) {
  void * returnValue;
  while(1) {

    pthread_mutex_lock(&mutex);
    int fd = workQueue[nextPop];
    nextPop = (nextPop + 1) % 8;
    filled = filled - 1;

    
    if(filled != 0) {
      pthread_cond_broadcast(&popSpace);
    }
    else {
      pthread_cond_wait(&popSpace, &mutex);
    }
    pthread_mutex_unlock(&mutex);
  }
  return returnValue;
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

  filled = 0;
  nextPush = 0;  
  nextPop = 0;

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_int(&openSpace);
  pthread_cond_int(&popSpace);
  
  pthread_t dispatcherThread;
  pthread_t workerThreads[threadDefault];

  int status = pthread_create(&dispatcherThread, NULL,
			      initialize_worker_thread, (void *)0);

  if(status != 0) {
    printf("Error %d creating dispather thread. \n", status);
    exit(1);
  }
  long i;
  for(i = 0; i < 1; i++) {
    status = pthread_create(&workerThreads[i], NULL,
			      initialize_to_server, (void *)i);
    if(status != 0) {
      printf("Error %d creating worker thread number: %l. \n", status, i + 1);
      exit(1);
    }
  }

  pthread_join(dispatcherThread, NULL);

  for(i = 0; i < threadDefault; i ++) {
    pthread_join(workerThreads[i], NULL);		 
  }
  
  return 0;
}
