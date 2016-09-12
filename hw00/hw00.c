// File: hw00/hw00.c
// Author: Robert Lowman
// Program for hw00 of CS364 Operating Systems

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[]) {

  int size;

  int reverse = 0;

  int skip = 0;

  printf("Enter the amount of integers you are later going to enter:");
  scanf("%d", &size);

  int * list = malloc(sizeof(int) * size);

  int i;
  
  for(i = 0; i < size; i = i + 1) {
    int temp;
    scanf("%d", &temp);
    list[i] = temp;
  }

  if(argc == 2) {
    if(strcmp(argv[1], "-r") == 0) {
      reverse = 1;
    }
    else {
      printf("Error: Invalid command");
      skip = 1;
    }
  }

  int index;
  
  if(skip == 0) {
    if(reverse == 0) {
      for(index = 0; index < size; index = index + 1) {
        int current = list[index];
	printf("%d", current);
      }
    }
    else {
       for(index = size; index > 0; index = index - 1) {
        int current = list[index];
	printf("%d", current);
      }
    }
  }
  return 0;
}
