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
    write(STDOUT_FILENO,error,strlen(error));
    exit(EXIT_FAILURE);
  }
  while(c!='\n'){
    if(arr.size >=arr.capacity){
      arr.capacity *= 2;
      arr.string = (char*)realloc(arr.string,sizeof(arr.capacity));
    }

    arr.string[arr.size++] = c;
    
    ssize_t bytesRead = read(STDIN_FILENO,&c,sizeof(char)); 
    if(bytesRead <= 0){
      char *error = strerror(errno);
      write(STDOUT_FILENO,error,strlen(error));
      exit(EXIT_FAILURE);
  } }

  return arr.string;
}
