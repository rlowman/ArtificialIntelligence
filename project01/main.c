// File: project01/main.c
// Author: Robert Lowman
// Implementation of the check-sum server for project01

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <checksum-client.c>
#include <proj01-networking.c>
#include <proj01-networking.h>

int selectThreadCount() {
  int * returnValue;
  printf("Enter the amount of worker threads you wish to use: ");
  scanf("%d", &returnValue);
  return returnValue;
}

int selectPort() {
  int * returnValue;
  printf("Enter the port number you wish to use: ");
  scanf("%d", &returnValue);
  return returnValue;
}

int main(int argc, char * argv[]) {
  int threadDefault = 4;
  int port; // Set to default here
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

  pthread_t threads[threadDefault];
  while(true) {
    
  }
  return 0;
}
