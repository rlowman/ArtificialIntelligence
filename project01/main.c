// File: project01/main.c
// Author: Robert Lowman
// Implementation of the check-sum server for project01

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include "proj01-networking.h"

#define BUFFER_SIZE 1024

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
 * Connects the dispatcher thread to the server.
 * 
 * @param foo Unused.
 * @return Unused.
 **/
void * initialize_to_server(void * foo) {
  printf("Entered dipathcer thread");
  int listen_socket = server_initialize_networking(port);
  printf("got socket");
  int currentFileDescriptor;
  void * returnValue;
  while(1) {

    printf("Dispatcher loop");
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
  char buffer[BUFFER_SIZE + 1];
  int bytesRead;
  int serial;
  int size;
  int fd;
  int checksumFile;
  long checksum;
  int bytesWritten;
  while(1) {
    printf("worker thread loop");
    pthread_mutex_lock(&mutex);
    fd = workQueue[nextPop];
    nextPop = (nextPop + 1) % 8;
    filled = filled - 1;
    if(filled != 0) {
      pthread_cond_broadcast(&popSpace);
    }
    else {
      pthread_cond_wait(&popSpace, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    memset(buffer, '\0', BUFFER_SIZE + 1);
    bytesRead = read(fd, buffer, BUFFER_SIZE);
    if(bytesRead == 0) {
      printf("Unable to read from File Descriptor: %d", fd);
    }
    else {
      char dest[bytesRead + 1];
      strncpy(dest, buffer, bytesRead + 1);
      serial = atoi(dest);
      checksumFile = open(dest, O_RDONLY);
      checksum = 0;
      bytesRead = read(checksumFile, buffer, BUFFER_SIZE);
      while(bytesRead > 0) {
	for(int i = 0; i < bytesRead; i ++) {
	  checksum = checksum + (long)buffer[i];
	  bytesRead = read(checksumFile, buffer, BUFFER_SIZE);
	}
      }
      size = snprintf(buffer, BUFFER_SIZE, "%d %s %ld", serial,
		      dest, checksum);
      bytesWritten = write(fd, buffer, size);
      if(bytesWritten == 0) {
	printf("Could not write back to socket %d", fd);
      }
      close(fd);
    }
  }
  return returnValue;
}

int main(int argc, char * argv[]) {
  int threadDefault = 1;
  printf("Here");
  /* if(argc == 2) { */
  /*   if(strcmp(argv[1], "-p") == 0) { */
  /*     port = selectPort(); */
  /*   } */
  /*   else if(strcmp(argv[1], "-t") == 0) { */
  /*     threadDefault = selectThreadCount; */
  /*   } */
  /* } */
  /* else if(argc == 3) { */
  /*   if(strcmp(argv[1], "-p") == 0 || */
  /*      strcmp(argv[2], "-p") == 0) { */
  /*     port = selectPort(); */
  /*   } */
  /*   if(strcmp(argv[1], "-t") == 0 || */
  /*      strcmp(argv[2], "-t") == 0) { */
  /*     threadDefault = selectThreadCount; */
  /*   } */
  /* } */

  filled = 0;
  nextPush = 0;  
  nextPop = 0;

  printf("created global variables");
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&openSpace, NULL);
  pthread_cond_init(&popSpace, NULL);

  printf("created mutexes");
  pthread_t dispatcherThread;
  pthread_t workerThreads[threadDefault];

  int status = pthread_create(&dispatcherThread, NULL,
			      initialize_to_server, (void *)0);

  printf("created dispatcher thread");

  if(status != 0) {
    printf("Error %d creating dispather thread. \n", status);
    exit(1);
  }
  long i;
  for(i = 0; i < threadDefault; i++) {
    status = pthread_create(&workerThreads[i], NULL,
			      initialize_worker_thread, (void *)i);
    if(status != 0) {
      printf("Error %d creating worker thread number: %ld. \n", status, i + 1);
      exit(1);
    }
  }

  pthread_join(dispatcherThread, NULL);

  for(i = 0; i < threadDefault; i ++) {
    pthread_join(workerThreads[i], NULL);		 
  }
  
  return 0;
}
