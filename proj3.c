/*
Class: CPSC 346-01
Team Member 1: Matt Carter
Team Member 2: N/A 
GU Username of project lead: mcarter6
Pgm Name: proj3.c 
Pgm Desc: exploraiton of the proc file system 
Usage: 1) standard:  ./a.out -s 
Usage: 2) history:  ./a.out -h 
Usage: 3) load:  ./a.out -l 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void standard();
void history();
void load();

int main(int argc, char* argv[])
{
 if (argc != 2)
  {
   fprintf(stderr, "Error: Options required\n"); 
   fprintf(stderr, "usage: ./a.out -s|-h|-l\n\n"); 
   exit(EXIT_FAILURE);
  }
  
 if (!strcmp(argv[1],"-s"))
  standard();
 if (!strcmp(argv[1],"-h"))
  history();
 if (!strcmp(argv[1],"-l"))
  load();
}
/*
pre: none
post: displays CPU vendor_id, model name, and OS version
*/
void standard()
{
 char ch;
 FILE* ifp;
 char str[80];

 /*
 I've deliberately used two different mechanisms for writing to the console.  
 Use whichever suits you.
 strstr locates a substring
 */

 ifp = fopen("/proc/cpuinfo","r");
 while (fgets(str,80,ifp) != NULL)
  if (strstr(str,"vendor_id") || strstr(str,"model name"))
   puts(str);
 fclose (ifp);

 ifp = fopen("/proc/version","r");
 while ((ch = getc(ifp)) != EOF)
  putchar(ch);
 fclose (ifp);
}

/*
pre: none
post: displays time since the last reboot (DD:HH:MM:SS), time when the system 
      was last booted, number of processes that have been created since the
      last reboot.  
      Hint: strftime could be useful
*/
void history()
{
    char ch;
    FILE* ifp;
    char str[80];

    int sec;
    int min;
    int hour;
    int day;
    int temp;
    struct tm* info;

    ifp = fopen("/proc/uptime", "r");
    if (fgets(str, 9, ifp) != NULL) {
        sec = atoi(str)%60;
        min = atoi(str) / 60&60;
        hour = atoi(str) / 60/60%24;
        day = atoi(str)/60/60/24;
        printf("Time since last reboot: %d:%d:%d:%d\n", day, hour, min, sec);
    }
    fclose (ifp);

    time_t currTime;
    time(&currTime);
    currTime = currTime - atoi(str);
    info = localtime(&currTime);
    strftime(str, 80, "%x at %I: %M %p", info);
    printf("System booted on: %s\n", str);

    ifp = fopen("/proc/stat","r");
    while (fgets(str,80,ifp) != NULL)
        if (strstr(str,"processes"))
            puts(str);
    fclose (ifp);
}

/*
pre: none
post: displays total memory, available memory, load average (avg. number of processes over the last minute) 
*/
void load()
{
    char ch;
    FILE* ifp;
    char str[80];

    ifp = fopen("/proc/meminfo","r");
    while (fgets(str,80,ifp) != NULL)
        if (strstr(str,"MemTotal") || strstr(str,"MemAvailable"))
            puts(str);
    fclose (ifp);

    ifp = fopen("/proc/loadavg","r");
    if (fgets(str,4,ifp) != NULL) {
        printf("Load average is: %s\n", str);
    }
    fclose (ifp);
}
