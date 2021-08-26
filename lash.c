#include "lash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include "basics.h"
#include "builtin.h"
#include "prompt.h"
#include "input.h"
#include "glob.h"
#include "lib/string/strlib.h"

int main(int argc, char **arcv) {

    init_glob();

    set_prompt("\033[033m[%P]\033[0m-(%u)-{%t} \n >  ");

    // Run loop
    lash_loop();

    return EXIT_SUCCESS;
}

void lash_loop(void) {

    char *line;
    char **args;
    char **fileredirect = malloc(2 * sizeof(char*));
    char ***pipeargs = (char ***)malloc(LASH_TOK_BUFSIZE * sizeof(char**) ); 
    int special_args[4]= {0,0,0,0};

    int count;
    int status = 1;
    do {
        for (int i = 0; i< LASH_TOK_BUFSIZE; i++) {
            pipeargs[i] = (char **) malloc(LASH_TOK_BUFSIZE * sizeof(char *));
        }
        // writing the prompt
        write_prompt();
        count = 0;
        line = lash_read_line();
        args = lash_split_line(line,&count);
        
        args = lash_eval_args(args,&count,special_args, fileredirect,pipeargs);// Evaluate

        if (args != NULL) 
            lash_execute_shell(args,special_args,fileredirect,pipeargs);// Proceed
        //status = lash_execute(args);
        cleanup();
        free(line);
        free(args);
    } while (status);
}

/*
 *  Evaluate Args after Splitting
 *  Use: This is where we figure out things like piping, input/output redirection, and setting up things for execution
 *  Note: fileredirect[0,1] --> 0 = input, 1 = output. pipeargs = 3d array where each pipe is an array of arguements
 */
char **lash_eval_args(char **args, int *count, int *special_args, char **fileredirect, char ***pipeargs) {
    char ** new_args= malloc(LASH_TOK_BUFSIZE * sizeof(char*));
    int i;
    int idx = 0;
    int idx2 = 0;
    char *val;
    char *nextval;
    if (args[0] == NULL)
        return NULL;
    for (i = 0; i < *count; i++) {
        val = args[i];
        if (i == *count-1 && !strcmp(val,"&\0")) { // Check for & in end
            special_args[0] = 1;
        }
        else if (!strcmp(val,"&\0")) {
            printf("ERROR: Ampersand can only appear at the end of input"); 
            return NULL;
        } // Check for & otherwise
        else if (countchar(val,'>') > 0  ) {                   // Check for >
            if (countchar(val,'>') > 1 || special_args[1] > 1) {
                printf("ERROR: With >, too many redirects \n");
                return NULL;
            }
            else {
                if (args[i+1] == NULL) {
                    printf("ERROR: No output file detected \n");
                    return NULL;
                }
                nextval = args[i+1];
                if (countchar(nextval,'>') > 0) {
                    printf("ERROR: With >, too many redirects \n");
                    return NULL;
                }
                special_args[1] = special_args[1] + 1;
                fileredirect[1] = args[++i];
            }
        } 
        else if (countchar(val,'<') > 0  ) {                 // Check for <
            if (countchar(val,'<') > 1 || special_args[2] > 1) {
                printf("ERROR With <, too many redirects \n");
                return NULL;
            } 
            else {
                nextval = args[i+1];
                if (args[i+1] == NULL) {
                    printf("ERROR: No input file detected \n");
                    return NULL;
                }
                if (countchar(nextval,'<') > 0) {
                    printf("ERROR With <, too many redirects \n");
                    return NULL;
                }
                special_args[2] = special_args[2] + 1;
                fileredirect[0] = args[++i];
            }
        }
        else if (countchar(val,'|') > 0  ) {                 // Check for |
            idx2 = 0;
            while (i+1 < *count && args[i+1][0] != '|' && args[i+1] != NULL) {
                if (!strcmp(args[i+1],"<\0")) {
                    printf("ERROR: Input redirection must be the first command \n");
                    return NULL;
                }
                if (!strcmp(args[i+1],">\0")) {
                    if (args[i+2] == NULL || find_special_char(args[i+2]) != NULL || args[i+3] != NULL) {
                        printf("ERROR: output redirection must be the last command \n");
                        return NULL;
                    }
                    else {
                        special_args[1] = special_args[1] + 1;
                        fileredirect[1] = args[i+2];
                        i += 2;
                    }
                } 
                else {
                    pipeargs[special_args[3]][idx2++] = args[++i];
                }
            }
            special_args[3] = special_args[3] + 1;
        }
        else {
            new_args[idx++]=val;
        }
    }
    return new_args;
}

/*
 *  Launch a shell process
 *  Use: Launching a shell process with specified args, file redirection, and flags for piping
 *  Note: Piping is done in conjunction with execute shell function
 */
int lash_launch_process(char **args, int* special_args, char** fileredirect, int inputfd, int islast, int ispipe) {
    pid_t pid;
    int status;
    int inputfile,outputfile;
    inputfile = dup(inputfd);
    outputfile = dup(1);
    int piping[2];
    pipe(piping);
    pid = fork();
    if (pid == 0) {

        // Setup for piping
        if(inputfd == 0 && islast ==0 ){
            dup2(piping[1],outputfile);
        } else if (inputfd != 0 && islast == 0){
            dup2(inputfd,inputfile);
            dup2(piping[1],outputfile);
        } else {
            dup2(inputfd,inputfile);
        }

        // end setup for piping

        if (special_args[1] > 0 && islast) {   // > -- Redirect output
            if (special_args[1] > 1) {
                printf("ERROR: TOO MANY REDIRECTS");
                return -1;
            } 
            else {
                outputfile = creat(fileredirect[1], 0600);
            }
        } 

        if (special_args[2] > 0 && inputfd == 0) {   // > -- Redirect input
            if (special_args[2] > 1) {

                printf("ERROR: TOO MANY REDIRECTS");
                return -1;
            } 
            else {
                inputfile = open(fileredirect[0], O_RDONLY);
            }
        }

        dup2(inputfile, STDIN_FILENO);
        dup2(outputfile, STDOUT_FILENO);

        if (execvp(args[0], args) == -1) {
            perror("lash");
        }

        exit(-1);
    } else if (pid > 0) {   
        if(!ispipe && special_args[0] == 0) {
            do {
                waitpid(pid, &status, WUNTRACED);
            }
            while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

        if(inputfd != 0) 
            close(inputfd);
        close(piping[1]);
        if(islast)
            close(piping[0]);
    }
    return piping[0];
}

/*
 *  Execute Shell 
 *  Use: Execute Shell, Decide between builtins, Piping, or regular execution
 *  Note: Piping in conjunction with flags and launch shell process function
 */
int lash_execute_shell(char **args, int *special_args, char **fileredirect,char ***pipeargs) {

    int i;

    for (i = 0; i < lash_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    if(special_args[3] > 0) {
        int numpipes = special_args[3];
        int pipeindex = 0;
        int readpipe = 0;
        int isfirst = 1;
        while (numpipes > 0) {
            if(isfirst){
                readpipe = lash_launch_process(args,special_args,fileredirect,readpipe,0,1);
            }
            else{
                readpipe = lash_launch_process(pipeargs[pipeindex++],special_args,fileredirect,readpipe,0,1);
            }
            numpipes--;
            isfirst = 0;
        }
        lash_launch_process(pipeargs[pipeindex++],special_args,fileredirect,readpipe,1,1);

        for (i = 0; i < special_args[3]+1; ++i) 
            wait(NULL); 
    }
    else {   
        return lash_launch_process(args,special_args,fileredirect,0,1,0);
    }
    return 0;    
}

/*
 *  Utility Function
 *  Use: To cleanup any zombie processes without causing shell to hang
 */
void cleanup()
{     
    pid_t pid;
    pid = waitpid(-1,NULL,WNOHANG);
    while(pid != 0 && pid != -1){
        pid = waitpid(-1,NULL,WNOHANG);
    }
}
