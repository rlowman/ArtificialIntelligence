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
    while(bytesRead > 0) {
      short type = information->ut_type;
      if(type == USER_PROCESS) {
	time_t seconds = (time_t)information->ut_tv.tv_sec;
	char buffer[60];
	struct tm * time = localtime(&seconds);
	strftime(buffer, 60, "%Y-%m-%d %H:%M", time);
	printf("%s %s         %s (%s)\n", information->ut_user,
	       information->ut_line, buffer, information->ut_host);
      }
      bytesRead = read(fileNumber, information, sizeof(struct utmp));
    }
  }
  return 0;
}
