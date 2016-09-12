// File: hw00/hw00.c
// Author: Robert Lowman
// Program for hw00 of CS364 Operating Systems

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[]) {

  int size; //The first entered integer that is how many more integers to enter

  int reverse = 0; //Integer acting as a boolean that activates the reverse

  int skip = 0; //Skips over entire program if invalid command is given

  //Handles the arguments and determines if the program should reverse
  //or not and closes the program if an invalid command is given
  if(argc == 2) {
    if(strcmp(argv[1], "-r") == 0) {
      reverse = 1;
    }
    else {
      printf("Error: Invalid command");
      skip = 1;
    }
  }

  //Takes in the integers to print from standard input
  if(skip == 0) {
    printf("Enter the amount of integers you are later going to enter:\n");
    scanf("%d", &size);

    int * list = malloc(sizeof(int) * size);

    int i;
  
    for(i = 0; i < size; i = i + 1) {
      int temp;
      scanf("%d", &temp);
      list[i] = temp;
    }

    
    int index;

    printf("Output:\n");

    //Prints the output normally or in reverse depending on the given command
    if(reverse == 0) {
      for(index = 0; index < size; index = index + 1) {
        int current = list[index];
	printf("%d\n", current);
      }
    }
    else {
       for(index = size - 1; index >= 0; index = index - 1) {
        int current = list[index];
	printf("%d\n", current);
      }
    }
    free(list);
  }
  
  return 0;
}
