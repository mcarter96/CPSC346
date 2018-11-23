/*
Class: CPSC 346-01
Team Member 1: Matt Carter
Team Member 2: n/a
GU Username of project lead: mcarter6
Pgm Name: proj7.c
Pgm Desc: Implement the reader-writer with Linux pipes and threads
Usage: ./a.out <int>
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void* writer(void*);
void* reader(void*);
int is_prime(int);

struct params
{
    int num;
    int* passInfo;
};

typedef struct params param;

int main(int argc, char* argv[])
{
    pthread_t t1,t2,t3,t4;
    param p1,p2,p3,p4;
    int n, status;

    srand(time(0));
    int p[2], x;

    if (pipe(p) < 0) {
        exit(1);
    }

    p1.num = atoi(argv[1]);
    p1.passInfo = p;
    status = pthread_create(&t1, NULL, writer, (void*)&p1 );
    if (status != 0)
    {
        printf("Error in thread 1:  %d\n",status);
        exit(-1);
    }

    p2.num = 1;
    p2.passInfo = p;
    status = pthread_create(&t2, NULL, reader, (void*)&p2 );
    if (status != 0)
    {
        printf("Error in thread 2:  %d\n",status);
        exit(-1);
    }

    p3.num = 2;
    p3.passInfo = p;
    status = pthread_create(&t3, NULL, reader, (void*)&p3 );
    if (status != 0)
    {
        printf("Error in thread 3:  %d\n",status);
        exit(-1);
    }

    p4.num = 3;
    p4.passInfo = p;
    status = pthread_create(&t4, NULL, reader, (void*)&p4 );
    if (status != 0)
    {
        printf("Error in thread 4:  %d\n",status);
        exit(-1);
    }

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    return 0;
}

void* writer(void* param_in)
{

    printf("entering writer function\n");
    param* p = (param*)param_in;
    int* passInfo = p->passInfo;

    int numCount = 0;
    while (numCount < p->num) {
        int r = rand()%767;
        printf("Generated: %i\n", r);
        numCount++;
        write(passInfo[1], &r, sizeof(r));
    }
    close(passInfo[1]);
    printf("leaving writer function\n");
    pthread_exit(NULL);
}

int is_prime(int num)
{
    int i = 2;
    while (i < num)
    {
        if (num % i == 0)
            return 0;
        ++i;
    }
    return 1;
}

void* reader(void* param_in)
{
    printf("entering reader function\n");
    param* p = (param*)param_in;
    int* passInfo = p->passInfo;
    int number;

    while (read(passInfo[0], &number, sizeof(number)) > 0) {
        if (is_prime(number)) {
            printf("Reader %i: %i is prime\n", p->num, number);
        }
        sleep(5);
    }
    printf("leaving reader function\n");
    pthread_exit(NULL);
}