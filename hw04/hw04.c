// File: myls.c
// Author: Chad Hogg, Robert Lowman
// Program for CS364, King's College, Fall 2016, Homework 04.

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define BUFFER_SIZE 10000

/**
 * Print a particular file's modification time.
 *
 * @param mtime When the file was modified.
 **/
void print_mod_time( time_t mtime );

void print_all();

struct dirent *d;

DIR *dir;

int main(int argc, char * argv[]) {
  dir = opendir(".");
  if (dir == NULL) {
    printf ("Cannot open directory");
  }
  if(argc == 1) {
    while ((d = readdir(dir)) != NULL) {
      if(d->d_name[0] != '.'){
        printf ("%s\n", d->d_name);
      }
    }
  }
  if(argc == 2){
    if(strcmp(argv[1], "-a") == 0) {
      print_all();
    }
    else if (strcmp(argv[1], "-l") == 0 ) {

    }
    else {
      printf("Invalid arg: %s", argv[1]);
      return 1;
    }
  }
  else if(argc == 3){
    if(((strcmp(argv[1], "-l") == 0) && (strcmp(argv[2], "-a") == 0)) ||
      ((strcmp(argv[1], "-a") == 0) && (strcmp(argv[2], "-l") == 0))) {
        print_all();
    }
    else {
      printf("Invalid args");
      return 1;
    }
  }
  else if(argc > 3){
    printf("Too many arguments given");
    return 1;
  }
  closedir(dir);
  return 0;
}

void print_all(){
  while ((d = readdir(dir)) != NULL) {
    printf ("%s\n", d->d_name);
  }
}

void print_all_information(){

}

void print_mod_time( time_t mtime )
{
  char timeStr[BUFFER_SIZE];            // The string we will print
  time_t now = time( NULL );            // The current time
  struct tm splitModTime;               // User-friendly modification time
  struct tm splitNowTime;               // User-friendly current time
  localtime_r( &mtime, &splitModTime );
  localtime_r( &now, &splitNowTime );

  if( splitModTime.tm_year ==           // If modified this year, print month,
      splitNowTime.tm_year ) {          //   day, hours, minutes
    strftime( timeStr, BUFFER_SIZE, " %b %e %H:%M", &splitModTime );
  }
  else {                                // Otherwise print month, day, year
    strftime( timeStr, BUFFER_SIZE, " %b %e  %Y", &splitModTime );
  }

  printf( timeStr );                    // Actually print it
}
