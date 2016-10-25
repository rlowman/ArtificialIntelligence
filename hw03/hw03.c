// File: hw03.c
// Author: Robert Lowman
// A program that implements part of the ps command

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char * argv[]) {

  if(argc > 1) {
    for(int i = 1; i < argc; i ++){
      char buffer[BUFFER_SIZE + 1];
      memset(buffer, '\0', BUFFER_SIZE + 1);
      int size = snprintf(buffer, BUFFER_SIZE, "/proc/%s/stat", argv[i]);
      char openName[size + 1];
      openName[size] = '\0';
      strncpy(openName, buffer, size);
      int fd = open(openName, O_RDONLY);
      if(fd > 0) {	
	memset(buffer, '\0', BUFFER_SIZE + 1);
	read(fd, buffer, BUFFER_SIZE);
	strtok(buffer, " ");         //Skipping over the current process id
	char * name = strtok(NULL, " ");
	char * name_chopped = name + 1;
       	name_chopped[strlen(name_chopped) - 1] = '\0';
	printf("%s\t%s", name_chopped, argv[i]);
      
	strtok(NULL, " ");      //Skipping unecessary integer

	char * token = strtok(NULL, " ");
	int parentId = atoi(token);
	while(parentId != 0) {
	  printf("\t%d", parentId);
	  memset(buffer, '\0', BUFFER_SIZE + 1);
	  size = snprintf(buffer, BUFFER_SIZE,
			  "/proc/%d/stat", parentId);
	  char parentName[size + 1];
	  parentName[size] = '\0';
	  strncpy(parentName, buffer, size);
	  int parentFD = open(parentName, O_RDONLY);

	  memset(buffer, '\0', BUFFER_SIZE + 1);
	  read(parentFD, buffer, BUFFER_SIZE);
	  strtok(buffer, " ");     // Skipping over current process id
	  strtok(NULL, " ");       // Skipping over parent name
	  strtok(NULL, " ");       // Skipping over unecessary integer
	  token = strtok(NULL, " ");
	  parentId = atoi(token);
	  close(parentFD);
	}
	printf("\n");
	close(fd);
      }
      else {
	printf("Unable to open initial file");
      }
    }
  }
  return 0;
}


