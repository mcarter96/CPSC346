/*
Class: CPSC 346-01
Team Member 1: Matt Carter
Team Member 2: Katrina Baber
GU Username of project lead: mcarter6
Pgm Name: proj4.c
Pgm Desc: a basic shell program
Usage: ./a.out
Changes from original:
	->finished code
	->changed some variable names that were vague or annoying to type
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define EXTRA 80

char** getInput();
char** parseInput(char*);
void dispOutput(char**);
void execute(char*,char**);

void dispHistory(char **, int);
void save(char**, char*, int);
char* copy(char*);

char** parseHistory(char**, int);

int main(int argc, char* argv[]) {
    //A pointer to an array of pointers to char.  In essence, an array of
    //arrays.  Each of the second level arrays is a c-string. These hold
    //the command line arguments entered by the user.
    //as exactly the same structure as char* argv[]
    char **args;
    int size = 0;
    char** history = (char**)malloc(MAX_LINE);
    char *first;


    while (1) {
        printf("myShell> ");
        fflush(stdout);
        args = getInput();
        first = *args++;
        save(history, first, size);
        if (size <= 9)
            size++;

        execute(args[0], args);

        char **iter = args;
        char *iterchar = *iter++;

        if (args != NULL) {
            if (!strcmp(args[0], "quit")) { //quit -> end program
                printf("\n---> The end is here.\n");
                break;
            } else if (!(strcmp(args[0], "history"))) { //history -> display history
                dispHistory(history, size);
            } else if (!(strcmp(args[0], "cd"))) { //cd change directory
                chdir(args[1]);
            } else if (!(strcmp(args[0], "!!"))) {
                execute(history[2], args);
            } else if (args[0][0] == '!') {
                char *str = *args;
                *str++;
                int a = atoi(str);
                if (a < size + 1) {
                    char *temp = copy(history[a]);
                    char **newargs = parseInput(temp);
                    *newargs++;
                    execute(newargs[0], newargs);
                } else {
                    printf("\n--->INVALID INVALID INVALID\n");
                }
            }
        }
    }
    return 0;
}

/*
Reads input string from the key board.
invokes parseInput and returns the parsed input to main
*/
char** getInput() {
    char* input = (char*) malloc(MAX_LINE);
    char* buffer = input;
    char temp;
    char** output;
    while ((temp = getchar()) != '\n') {
        *input++  = temp;
    }
    *input = '\0';
    output = parseInput(buffer);
    return output;
}

/*
inp is a cstring holding the keyboard input
returns an array of cstrings, each holding one of the arguments entered at
the keyboard. The structure is the same as that of argv
Here the user has entered three arguements:
myShell>cp x y
*/
char** parseInput(char* inp) {
        char** args = (char**) malloc(MAX_LINE);
        char** start = args;
        *args++ = copy(inp);
        char* thing = strtok(inp, " ");
        *args++ = thing;
        while (thing != NULL) {
            thing = strtok(NULL, " ");
            *args++ = thing;
        }
        *args++ = "\0";
        args = start;
        return args;
}

/*
Displays the arguments entered by the user and parsed by getInput
*/
void dispOutput(char** args){
    int a = 0;
    while (args[a] != NULL) {
        printf("%s\n", args[a]);
        a++;
    }
}

//got majority of this code from example 13 on the 346 github
void execute (char* command, char** parameters) {
    pid_t pid;

    char** iter = parameters;
    char* first = *iter++;

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork Error");
    } else {
        if (pid == 0) {
            printf("Command: %s\n", command);
            printf("Parameters: %s\n", parameters[0]);
            execvp(command, parameters);
            exit(0);
        } else {
            wait(NULL);
        }
    }
}



// copys cstring
char* copy (char* origin) {
    char* copyTo = (char*)malloc(MAX_LINE);
    char* tempChar = copyTo;
    char* print = origin;
    char temp;
    temp = *print++;
    while(temp != '\0') {
        *copyTo = temp;
        temp = *print++;
        copyTo++;
    }
    return tempChar;
}


void save(char** history, char* newcs, int i) {
    while(i > 0) {
        history[i] = history[i - 1];
        i--;
    }
    history[0] = newcs;
}


void dispHistory(char** history, int i) {
    char** printHistory = history;
    int placeHolder = i;

    while(placeHolder > 1) {
        printHistory++;
        placeHolder--;
    }
    //considered solution brought up in class
    //where *** is used instead of **
    //and you don't have to loop through history
    while(i > 0)   {
        printf("%d: %s\n", i, *printHistory);
        printHistory--;
        i--;
    }
}

char** parseHistory(char** history, int i) {
    printf("a");
    char* cmd = copy(history[i]);
    printf("%s", cmd);
    return parseInput(cmd);
}