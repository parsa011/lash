#include "history.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/string/strlib.h"
#include "glob.h"

FILE *history_file;
char *history_file_path;
uint64 history_poiner;

/*
 *  Open history file from the user home path , and create if dosnt exitst
 */
void init_lash_history() {
    history_file_path = concat(2,user_home_path,HISTORY_FILE_NAME);
    history_file = fopen(concat(2,user_home_path,HISTORY_FILE_NAME),"w+");
}

/*
 *  Add a line to history file , if post == -1 will and to bottome of file
 */
void add_to_history(const char *line,int pos) {
}

/*
 *  Get a special line of history file
 */
void get_history(int line) {
}

/*
 *  Close history file 
 */
void close_history() {
    fclose(history_file);
}
