// File: catProgram.c
// Author: Robert Lowman
// A program that implements the cat program

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char * argv[]) { 
  int outFile;
  char buffer[BUFFER_SIZE + 1];
  buffer[BUFFER_SIZE] = '\0';
  int numRead;
  int lineCount = 1;
  int returnVal;
  int n = 0;
  int fileCount = argc - 1;

  if(strcmp(argv[fileCount], "-n") == 0 ||
     strcmp(argv[fileCount], "--number") == 0) {
    fileCount = fileCount - 1;
    n = 1;
  }
  
  int skip = 0;
  int i = 1;
  while(i <= fileCount && skip == 0) {
    int temp;
    if(strcmp(argv[i], "-") == 0) {
      temp = open(STDIN_FILENO, O_RDONLY);
    }
    else {
      temp = open(argv[i], O_RDONLY);
    }
    if(temp == -1) {         //If the given file was invalid, then give error
      printf("Error opening input file");
      skip = 1;
      returnVal = 2;
    }
    else {
      numRead = read(temp, buffer, BUFFER_SIZE);
      printf("%d", numRead);
      while(numRead > 0) {
	if(n == 1) {
	  char * p;
	  p = strchr(buffer, '/n');
	  while(p!=NULL) {
	    char temp[
	    p = strchr(p+1, 
	  }
        }
        else {
          printf("%s", buffer);
          for(int c = 0; c < 1024; c = c + 1) {
            buffer[c] = '\0';
          }
        }
        numRead = read(temp, buffer, BUFFER_SIZE);
      }
      i = i + 1;
    }
    close(temp);
  }
  return 0;
}
