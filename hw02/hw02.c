// File: hw02.c
// Author: Robert Lowman
// A program that implements the who command

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char * argv[]) {
  int fileNumber = open("/var/run/utmp", O_RDONLY);

  if(fileNumber == -1) {
    //error
  }
  else {
    struct utmp * information = malloc(sizeof(struct utmp));
    int bytesRead = read(fileNumber, utmp, sizeof(utmp));
    
  }
  return 0;
}
