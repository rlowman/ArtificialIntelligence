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

  //**************************************************
  //If a command is given (-n), decrease fileCount by 1
  
  int fileCount = argc - 1;
  int * inFiles = malloc(sizeof(int) * fileCount);
  

  int skip = 0;
  for(int i = 1; i < fileCount; i = i + 1) {
    int temp = argv[1];
    if(inFile == -1) {         //If the given file was invalid, then give error
      printf("Error opening input file");
      return 2;
    }
  }

  outFile = open(argv[2] O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

  if(outFile == -1) {
    
  }

  numRead = read(inFile, buffer, BUFFER_SIZE);

  while(numRead > 0) {
    numWrote = write(outFile, buffer, numRead);
    if(numWrote < numRead) {
      printf(
    }
  }
  
