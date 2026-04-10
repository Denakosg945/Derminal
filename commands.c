#include "headers/commands.h"
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>


char *join_path(const char *path_A,const char *path_B){
  size_t length = strlen(path_A) + strlen(path_B) + 2;
  char *buff = (char*)malloc(length*sizeof(char));

  if(buff == NULL){
    return NULL;
  }

  buff[0] = '\0';

  strcat(buff,path_A);
  strcat(buff,"/");
  strcat(buff,path_B);

  return buff;
}

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

  if(strcmp("..",nwd) == 0){
    //return the previous dir 
    char *tmp_cwd = (char*)malloc(sizeof(char)*strlen(cwd)+1);
    if(!tmp_cwd){
      return NULL;
    }
    strcpy(tmp_cwd,cwd);
    char *last_slash = strrchr(tmp_cwd,'/');

    if(last_slash != NULL){
      if(last_slash == tmp_cwd){
        tmp_cwd[1] = '\0';
      }else{
        *last_slash = '\0';
      }
    }
    
    return tmp_cwd;
  }else if(strcmp(".",nwd) == 0){
    char *tmp_cwd = (char*)malloc(sizeof(char)*strlen(cwd)+1);
    if(!tmp_cwd){
      return NULL;
    }
    strcpy(tmp_cwd,cwd);
    return tmp_cwd;      
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
      char *nwd_string = join_path(cwd,nwd);
      if(nwd_string ==NULL){
        char *error = strerror(errno);
        write(STDOUT_FILENO,error,strlen(error));
        closedir(directory);
        return NULL;
      }

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

  char *new_dir_name = join_path(cwd,name);
  //new_dir_name == NULL
  //
  if(!new_dir_name){
    write(STDOUT_FILENO,"Could not make directory...\n",strlen("Could not make directory...\n"));
    //Failed 
    return -1;
  }


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
  char *dir_to_del = join_path(cwd,name);
  //dir_to_del == NULL
  //
  if(!dir_to_del){
    write(STDOUT_FILENO,"Could not make directory...\n",strlen("Could not make directory...\n"));
    //Failed 
    return -1;
  }


  


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
      write(STDOUT_FILENO,error,strlen(error));
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

int cfile(char *name,char *cwd){
  if(name[0] == '/'){
    int created = open(name,O_WRONLY|O_CREAT|O_TRUNC, 0);
    if(created < 0){
      write(STDOUT_FILENO,name,strlen(name));
      write(STDOUT_FILENO,"open error :",strlen("open error :"));
      write(STDOUT_FILENO,strerror(errno),strlen(strerror(errno)));
      return -1;
    }
    close(created);
    return 0;
  }

  char *new_file_name = join_path(cwd,name);


  int created = open(new_file_name,O_WRONLY|O_CREAT|O_TRUNC,0);
  if(created < 0){
    write(STDOUT_FILENO,"open error :",strlen("open error :"));
    write(STDOUT_FILENO,strerror(errno),strlen(strerror(errno)));
    return -1;
  }
  free(new_file_name);
  close(created);
  return 0;
}

int start(const char *program,char *const argv[]){
  int id = fork();
  if(id == -1){
    char *error = strerror(errno);
    write(STDOUT_FILENO,error,strlen(error));
    return id;
  }else if(id == 0){
    if(execv(program,argv) == -1){
      char *error = strerror(errno);
      write(STDOUT_FILENO,error,strlen(error));
      write(STDOUT_FILENO,"\n",1);
      _exit(-1);
    }
  }else{
    int status;
    waitpid(id,&status,0);
    
  }


  return 0;
}


