#include "headers/commands.h"
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
//chd = change directory , nwd = new working directory
//
char *chd(const char *nwd,const char *cwd){
  DIR *directory = opendir(cwd);
  

  if(directory == NULL){ 
    char *error = strerror(errno);
    write(STDOUT_FILENO,"1",1);
    write(STDOUT_FILENO,error,strlen(error));
    return NULL; 
  }
  



  struct dirent *dir;

  do{

    //Read all directories and print them
    errno = 0;
    dir = readdir(directory);
    if(errno != 0){
      char *error = strerror(errno);
      write(STDOUT_FILENO,"2",1);
      write(STDOUT_FILENO,error,strlen(error));
      return NULL;
    }
    
    


    if(dir != NULL && (strcmp(nwd,dir->d_name)==0) ){
      char *nwd_string = (char*)malloc(sizeof(char)*strlen(nwd)+sizeof(char)*strlen(cwd));
      if(nwd_string ==NULL){
        char *error = strerror(errno);
        write(STDOUT_FILENO,"3",1);
        write(STDOUT_FILENO,error,strlen(error));
        return NULL;
      }
    

      strncat(nwd_string,cwd,strlen(cwd));
      strcat(nwd_string,"/");
      strncat(nwd_string,nwd,strlen(nwd));
      return nwd_string;
    }
  }while(dir != NULL);

  closedir(directory);
  return NULL;
}



//ls clone 
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
  
  if(closedir(directory) < 0){
    char *error = strerror(errno);
    write(STDOUT_FILENO,error,strlen(error));
    exit(EXIT_FAILURE);
  }

}
