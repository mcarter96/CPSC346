/*
Class: CPSC 346-01
Team Member 1: Matt Carter
Team Member 2: n/a
GU Username of project lead: mcarter6
Pgm Name: proj5.c
Pgm Desc: Implements Peterson's Algorithm with shared memory
*/

#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int time_parent;
int time_child;
int time_parent_non_cs;
int time_child_non_cs;

int shmid, value, turn;
int* values;

void child(int, int);
void parent(int, int);
void cs(char, int);
void non_crit_sect(int);

int main(int argc, char* argv[]) {
    if (argc == 1) {
        time_parent = 1;
        time_child = 3;
        time_parent_non_cs = 1;
        time_child_non_cs = 5;
    }

    shmid = shmget(0, 5, 0777 | IPC_CREAT);
    values = (int*)shmat(shmid,0,0);

    int proc1 = 0; // Child
    int proc0 = 1; // Parent
    turn = 0;

    values[0] = proc0;
    values[1] = proc1;
    values[2] = turn;
    values[3] = 1; //parent flag
    values [4] = 1; //child flag

    if (fork() == 0) // Parent code
    {
        parent(time_parent, time_parent_non_cs);
    } else {
        child(time_child, time_child_non_cs);
    }

    // Waiting for both processes to exit
    sleep(values[3] || values[4]);
    shmdt(values);
    shmctl(shmid, IPC_RMID, 0);
    printf("Function has run completely\n");
    return 0;
}

void parent(int time_parent, int time_parent_non_cs) {
    for (int i = 0; i < 10; i++) {
        //protect this
        values[0] = 1;
        values[2] = 1;
        while (values[1] && values[2]);
        cs('P', time_parent);
        values[0] = 0;
        non_crit_sect(time_parent_non_cs);
    }
    values[3] = 0;
}

void child(int time_child, int time_child_non_cs) {
    for (int i = 0; i < 10; i++) {
        //protect this
        values[1] = 1;
        values[2] = 1;
        while (values[0] && !values[2]);
        cs('C', time_child);
        values[1] = 0;
        non_crit_sect(time_child_non_cs);
    }
    values[4] = 0;
}

void cs(char process, int time_crit_sect) {
    if (process == 'P')
    {
        printf("parent in critical section\n");
        sleep(time_crit_sect);
        printf("parent leaving critical section\n");
    }
    else
    {
        printf("child in critical sction\n");
        sleep(time_crit_sect);
        printf("chile leaving critical section\n");
    }
}

void non_crit_sect(int time_non_crit_sect)
{
    sleep(time_non_crit_sect);
}