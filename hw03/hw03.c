// File: hw03.c
// Author: Robert Lowman
// A program that implements part of the ps command

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char * argv[]) {

  if(argc > 1) {
    char buffer[BUFFER_SIZE + 1];
    memset(buffer, '\0', BUFFER_SIZE + 1]
    int size = snprintf(buffer, BUFFER_SIZE, "%s/stat", argv[0]);
    char openName[size + 1];
    openName[size] = '/0';
    strncpy(openName, buffer, size);
    int fd = open(openName, O_RDONLY);
    for(int i = 1; i < argc; i ++) {
      int processId = argv[i];
    }
    for(int i = 1; i < 
  }
  return 0;
}
