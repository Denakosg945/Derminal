#ifndef COMMANDS_HEADER
#define COMMANDS_HEADER

void list(const char *wd);
char *chd(const char *nwd,const char *cwd);
int mkd(const char *name,const char *cwd);
int rmd(const char *name,const char *cwd);
int start(const char *program,char *const argv[]);
int cfile(char *name, char *cwd);
int rfile(char *name, char*cwd);

#endif
