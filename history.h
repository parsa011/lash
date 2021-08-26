#include <stdio.h>
#include "basics.h"

#define HISTORY_FILE_NAME ".lashhistory"

extern FILE *history_file;
extern char *history_file_path;
extern uint64 history_pointer;

void clear_history();
void where_history();
char *current_history();
void history_set_pos();
void init_lash_history();
void add_to_history(const char *,int);
void get_histroy(int);
void remove_history(int);
void close_history();
