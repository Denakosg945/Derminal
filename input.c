#include <stdlib.h>
#include "headers/input.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
// DEFAULT_CMD_CAPACITY 
//

char *command(){
  Array arr;
  arr.size = 0;
  arr.capacity = DEFAULT_CMD_CAPACITY;
  arr.string = (char*)malloc(sizeof(char)*DEFAULT_CMD_CAPACITY);
  
  if(arr.string == NULL){
    char *error = strerror(errno);
    write(STDOUT_FILENO,error,strlen(error));
    exit(EXIT_FAILURE);
  }


  char c;
  ssize_t bytesRead = read(STDIN_FILENO,&c,sizeof(char));
  
  if(bytesRead <= 0){
    
    
    char *error = strerror(errno);
  }

  while(c!='\n'){
    if(arr.size >=arr.capacity){
      arr.capacity *= 2;
      char *tmp= (char*)realloc(arr.string,arr.capacity*sizeof(char));
      if(tmp == NULL){
        arr.string[0] = '\0';
        return arr.string;
      }
      arr.string = tmp;
    }

    arr.string[arr.size++] = c;
    
    bytesRead = read(STDIN_FILENO,&c,sizeof(char)); 
    if(bytesRead <= 0){
      char *error = strerror(errno);
      write(STDOUT_FILENO,error,strlen(error));
      exit(EXIT_FAILURE);
  } }

  arr.string[arr.size] = '\0';
  
     
  return arr.string;
}

char **tokenize_command(char *cmd){

  if(cmd == NULL){
    return NULL;
  }

  char *delimiter = " ";


  int capacity = 2;
  int counter = 0;
  char **tokens = (char**)malloc(sizeof(char*)*capacity);
  

  char *token = strtok(cmd,delimiter);
  while(token != NULL){
    if(counter >= capacity){
      capacity *= 2;
      tokens = realloc(tokens,sizeof(char*)*capacity);

      if(!tokens){
        free(tokens);
        return NULL;
      }

    }

    tokens[counter++] = token;
    token = strtok(NULL,delimiter);
  }

  tokens = (char**)realloc(tokens,sizeof(char)*counter+1);
  tokens[counter] = '\0';

  return tokens;
}
