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

  char **tokens = (char**)malloc(sizeof(char*));
  int counter = 0;
  tokens[counter] = strtok(cmd,delimiter);
  counter++;

  char *token;
  while((token = strtok(NULL,delimiter)) != NULL){
    tokens[counter] = token;
    counter++;
  }

  return tokens;
}
