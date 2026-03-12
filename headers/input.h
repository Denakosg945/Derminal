#ifndef INPUT_HEADER
#define INPUT_HEADER

#define DEFAULT_CMD_CAPACITY 20

char *command();

typedef struct {
  ssize_t size;
  ssize_t capacity;
  char *string;
}Array;

#endif

