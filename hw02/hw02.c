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
#include <time.h>

int main(int argc, char * argv[]) {
  int fileNumber = open("/var/run/utmp", O_RDONLY);

  if(fileNumber == -1) {
    //error
  }
  else {
    struct utmp * information = malloc(sizeof(struct utmp));
    int bytesRead = read(fileNumber, information, sizeof(struct utmp));
    time_t seconds = (time_t)information->ut_tv.tv_sec;
    char buffer[60];
    struct tm * time = localtime(&seconds);
    strftime(buffer, 60, "%Y-%m-%d %H:%M", time);
    printf("Name: %s\n", information->ut_user);
    printf("Terminal: %s\n", information->ut_line);
    printf("Time: %s\n", buffer);
    printf("Remote Computer: %s", information->ut_host);
  }
  return 0;
}
