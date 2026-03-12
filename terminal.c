#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "headers/commands.h"
#include "headers/input.h"

#define WD_STRING_SIZE 1024



size_t my_strlen(char *string){
  int c = 0;
  while(string[c]!='\0'){
    c++;
  }

  return ++c;
}

int main(int argc, char** argv){
  //working directory string size + '\0' 
  char wd[WD_STRING_SIZE+1];
  //REPL 
  while(1){
    getcwd(wd,WD_STRING_SIZE);
    wd[WD_STRING_SIZE] = '\0';


    write(STDOUT_FILENO,wd,my_strlen(wd));
    write(STDOUT_FILENO,"->",sizeof("->"));
    
    char *com = command();

    if(strcmp(com,"`") == 0){
      free(com);
      break;
    }else if(strcmp(com,"list") == 0){
      list(wd);
    }      
  }

  
  return 0;
}


