#include "history.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "lib/string/strlib.h"
#include "lib/file/filelib.h"
#include "glob.h"

FILE *history_file;
char *history_file_path;
uint64 history_poiner;

/*
 *  Open history file from the user home path , and create if dosnt exitst
 */
void init_lash_history() {
    history_file_path = concat(2,user_home_path,HISTORY_FILE_NAME);
    history_file = fopen(concat(2,user_home_path,HISTORY_FILE_NAME),"a+");
}

/*
 *  Add a line to history file , if post == -1 will and to bottome of file
 */
void add_to_history(const char *line,int pos) {
    // TODO : before add we have to check if last added command is not equals to new line
    if (pos == -1)
        pos = file_linecount(history_file);
    fseek(history_file,10,SEEK_SET);
    fprintf(history_file,"%s\n",line);
    savechanges();
}

/*
 *  Get a special line of history file
 */
void get_history(int line) {
}

/*
 *  Remove special line of history
 */ 
void remove_history(int line) {
}

void history_set_pos() {
}

char *current_history() {
    return "";
}

/*
 *  Close history file 
 */
void close_history() {
    fclose(history_file);
}

/*
 *  Add changes to history file
 */
void savechanges() {
    fflush(history_file);
}

/*
 *  make history epmty
 */
void clear_history() {
}
