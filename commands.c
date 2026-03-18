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
  DIR *directory;
  
  //  write(STDOUT_FILENO,nwd,strlen(nwd));
  if(nwd[0] == '/'){
    directory = opendir(nwd);
    if(directory == NULL){
      write(STDOUT_FILENO,"No such directory!\n",strlen("No such directory!\n"));
      char *dire = (char*)malloc(sizeof(char)*strlen(cwd)+1);
      if(!dire){
        return NULL;
      }
      strcpy(dire,cwd);
      return dire;
    }

    char *dire = (char*)malloc(sizeof(char)*strlen(nwd)+1);
    strcpy(dire,nwd);
    closedir(directory);
    return dire;
  }
  
  directory = opendir(cwd); 

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
    
    


    if(dir != NULL && strcmp(nwd,dir->d_name)==0 && dir->d_type == DT_DIR){
      char *nwd_string = (char*)malloc(sizeof(char)*strlen(nwd)+sizeof(char)*strlen(cwd)+2);
      if(nwd_string ==NULL){
        char *error = strerror(errno);
        write(STDOUT_FILENO,"3",1);
        write(STDOUT_FILENO,error,strlen(error));
        closedir(directory);
        return NULL;
      }

      nwd_string[0] = '\0';
    

      strncat(nwd_string,cwd,strlen(cwd));
      strcat(nwd_string,"/");
      strncat(nwd_string,nwd,strlen(nwd));
      
      closedir(directory);
      //write(STDOUT_FILENO,nwd_string,strlen(nwd_string));
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
    return ;
  }
  
  struct dirent *dir;

  do{
    //Read all directories and print them
    errno = 0;
    dir = readdir(directory);
    if(errno != 0){
      char *error = strerror(errno);
      write(STDOUT_FILENO,"Invalid directory!\n",strlen("Invalid directory!\n"));
      return ;
    }
    
    if(dir != NULL){
      if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0){
        continue;
      }
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
