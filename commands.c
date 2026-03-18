#include "headers/commands.h"
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

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
    
      nwd_string = realloc(nwd_string,strlen(nwd_string)+1);
      nwd_string[strlen(nwd_string)+1] = '\0';

      closedir(directory);
      //write(STDOUT_FILENO,nwd_string,strlen(nwd_string));
      return nwd_string;
    }
  }while(dir != NULL);
  
  closedir(directory);
  return NULL;
}

int mkd(const char *name,const char *cwd){
  if(name[0] == '/'){
    int new_dir = mkdir(name,0777);
    if(new_dir == -1){
      char *error = strerror(errno);
      write(STDOUT_FILENO,error,strlen(error));
      return -1;
    }

  }

  char *new_dir_name = (char*)malloc(strlen(name)*sizeof(char)+strlen(cwd)*sizeof(char)+2);
  //new_dir_name == NULL
  //
  if(!new_dir_name){
    write(STDOUT_FILENO,"Could not make directory...\n",strlen("Could not make directory...\n"));
    //Failed 
    return -1;
  }



  new_dir_name[0] = '\0';
  strncat(new_dir_name,cwd,strlen(cwd));
  strcat(new_dir_name,"/");
  strncat(new_dir_name,name,strlen(name));
  

  write(STDOUT_FILENO,new_dir_name,strlen(new_dir_name));
  //See man page 7 for inode for magic number 0777
  int new_dir = mkdir(new_dir_name,0777);
  if(new_dir == -1){
    char *error = strerror(errno);
    write(STDOUT_FILENO,error,strlen(error));
    free(new_dir_name);
    return -1;
  }
  free(new_dir_name);
  return 1;
}

int rmd(const char *name,const char *cwd){
  if(name[0] == '/'){
    
    int removed_dir = rmdir(name);
    if(removed_dir == -1){
      char *error = strerror(errno);
      write(STDOUT_FILENO,error,strlen(error));
      return -1;
    }

  }
  char *dir_to_del = (char*)malloc(strlen(name)*sizeof(char)+strlen(cwd)*sizeof(char)+2);
  //dir_to_del == NULL
  //
  if(!dir_to_del){
    write(STDOUT_FILENO,"Could not make directory...\n",strlen("Could not make directory...\n"));
    //Failed 
    return -1;
  }



  dir_to_del[0] = '\0';
  strncat(dir_to_del,cwd,strlen(cwd));
  strcat(dir_to_del,"/");
  strncat(dir_to_del,name,strlen(name));
  

  write(STDOUT_FILENO,dir_to_del,strlen(dir_to_del));

  int removed_dir = rmdir(dir_to_del);
  if(removed_dir == -1){
    char *error = strerror(errno);
    write(STDOUT_FILENO,error,strlen(error));
    free(dir_to_del);
    return -1;
  }
  free(dir_to_del);
  return 1;
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
