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
#include <utmp.h>

int main(int argc, char * argv[]) {
  int fileNumber = open("/var/run/utmp", O_RDONLY);

  if(fileNumber == -1) {
    //error
  }
  else {
    struct utmp * information = malloc(sizeof(struct utmp));
    int bytesRead = read(fileNumber, information, sizeof(struct utmp));
    printf("Name: %s", information->ut_user);
    printf("Terminal: %s", information->ut_line);
    printf("Time: %d", information->ut_tv->tv_sec);
    printf("Remote Computer: %s", information->ut_host);
  }
  return 0;
}
