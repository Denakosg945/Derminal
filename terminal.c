#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "headers/commands.h"
#include "headers/input.h"

#define WD_STRING_SIZE 1024



int main(int argc, char** argv){
  //working directory string size + '\0' 
  char *wd = (char*)malloc(sizeof(char)*WD_STRING_SIZE+1);
  getcwd(wd,WD_STRING_SIZE);

  //REPL 
  while(1){

    
    write(STDOUT_FILENO,wd,strlen(wd));
    write(STDOUT_FILENO,"->",2);
  
    
    
    char *com = command();

    
    

    char **tokens = tokenize_command(com);
    

    size_t token_count = 0;
    while(tokens[token_count] != NULL) token_count++;
      if(token_count <= 0){
//        write(STDOUT_FILENO, "Enter a valid path!\n", 21);
        // free memory
        free(com);
        for(size_t i=0;i<token_count;i++) free(tokens[i]);
          free(tokens);
          continue;
    }

    if(token_count == 0){
      continue;
    }
    

    

    if(strcmp(tokens[0],"`") == 0){
      break;
    }else if(strcmp(tokens[0],"list") == 0){
      list(wd);
    }else if(strcmp(tokens[0],"chd")== 0){
      if(token_count <= 1){
        write(STDOUT_FILENO,"Enter a valid path!\n",sizeof("Enter a valid path!\n"));
        continue;
      }
      char *temp;
      if(token_count == 2){
        temp = chd(tokens[1],wd);
      }
      if(temp != NULL) {
        wd = temp;
      }else{
        write(STDOUT_FILENO,"No such directory\n",sizeof("No such directory\n"));
      }
    }else if(strcmp(tokens[0], "mkd") == 0){
      if(token_count <= 1){
        write(STDOUT_FILENO,"Enter a valid path!\n",sizeof("Enter a valid path!\n"));
        continue;
      }

      if(mkd(tokens[1],wd) == -1){
        write(STDOUT_FILENO,"Enter a valid path!\n",sizeof("Enter a valid path!\n"));
        continue;
      }
      write(STDOUT_FILENO,"Directory successfully created!\n",sizeof("Directory successfully created!\n"));
     }else if(strcmp(tokens[0], "rmd") == 0){
      if(token_count <= 1){
        write(STDOUT_FILENO,"Enter a valid path!\n",sizeof("Enter a valid path!\n"));
        continue;
      }

      if(rmd(tokens[1],wd) == -1){
        write(STDOUT_FILENO,"Enter a valid path and make sure the directory is empty!\n",sizeof("Enter a valid path and make sure the directory is empty!\n"));
        continue;
      }
      write(STDOUT_FILENO,"Directory successfully deleted!\n",sizeof("Directory successfully deleted!\n"));
    }
    else{
      write(STDOUT_FILENO,"Unknown command...\n",sizeof("Unknown command...\n"));
    } 
  }

  
  return 0;
}


