#ifndef INPUT_HEADER
#define INPUT_HEADER

#define DEFAULT_CMD_CAPACITY 20

char *command();
char **tokenize_command(char *cmd);


typedef struct {
  ssize_t size;
  ssize_t capacity;
  char *string;
}Array;

#endif

