#include "headers/commands.h"
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void list(const char *wd){
  DIR *directory = opendir(wd);

  if(directory == NULL){ 
    char *error = strerror(errno);
    write(STDOUT_FILENO,error,strlen(error));
    exit(EXIT_FAILURE);
  }
  
  struct dirent *dir;

  do{
    //Read all directories and print them
    errno = 0;
    dir = readdir(directory);
    if(errno != 0){
      char *error = strerror(errno);
      write(STDOUT_FILENO,error,strlen(error));
      exit(EXIT_FAILURE);
    }
    
    if(dir != NULL){
      
      ssize_t bytesWrote = write(STDOUT_FILENO,dir->d_name,strlen(dir->d_name));
      if(bytesWrote <=0){
        char *error = strerror(errno);
        write(STDOUT_FILENO,error,strlen(error));
        exit(EXIT_FAILURE);
      }
      bytesWrote = write(STDOUT_FILENO,"\n",1);
      if(bytesWrote <=0){
        char *error = strerror(errno);
        write(STDOUT_FILENO,error,strlen(error));
        exit(EXIT_FAILURE);
      }

    }

  } while(dir != NULL);

}
