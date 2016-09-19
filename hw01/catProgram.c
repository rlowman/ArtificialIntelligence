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
  int outFile
  char buffer[BUFFER_SIZE];
  int numRead;
  int numWrote;
  int returnVal;
  int n = 0;
  int fileCount = argc - 1;

  if(strcmp(argv[fileCount], "-n") == 0 ||
     strcmp(argv[fileCount], "--number") == 0) {
    fileCount = fileCount - 1;
    n = 1;
  }
  int * inFiles = malloc(sizeof(int) * fileCount);
  
  int skip = 0;
  int i = 1;
  int fileIndex = 0;
  while(i < fileCount && skip == 0) {
    int temp = open(argv[i], O_RDONLY);
    if(temp == -1) {         //If the given file was invalid, then give error
      printf("Error opening input file");
      skip = 1;
      returnVal = 2;
    }
    else {
      inFiles[fileIndex] = temp;
      fileIndex = fileIndex + 1;
      i = i + 1;
    }
  }
  
  for(int index = 0; index < fileCount; index = index + 1) {
    numRead = read(inFile, buffer, BUFFER_SIZE);
    while(numRead > 0) {
      
      if(numWrote < numRead) {
        printf();
      }
    }
  }
}
