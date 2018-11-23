/*
Class: CPSC 346-01
Team Member 1: Matt Carter
Team Member 2: n/a
GU Username of project lead: mcarter6
Pgm Name: proj8.c
Pgm Desc: Constructing a virtual memory simulator
Usage: ./a.out BACKING_STORE.bin addresses.txt
*/

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define TLB_SIZE 16
#define PAGES 256
#define PAGE_MASK 255
#define PAGE_SIZE 256
#define OFFSET_BITS 8
#define OFFSET_MASK 255
#define MEMORY_SIZE PAGES * PAGE_SIZE

struct tlbentry
   {
    unsigned char page_number;
    unsigned char frame_number;
    int tlb_ref;
   };
typedef struct tlbentry tlbentry;



int main(int argc, char *argv[]) {
    int x = 0;
    int pagetable[PAGES];
    tlbentry tlb[TLB_SIZE];
    signed char main_memory[MEMORY_SIZE];
    signed char *backing;
    char tempAddress[1000];
    int logical_address;
    int offset;
    int logical_page;
    int physical_page;
    int physical_address;
    int numPageFaults = 0;
    int numPageRefs = 1;
    int numTLBRefs = 1;
    int numTLBHits = 0;

    FILE *ifp = fopen(argv[2], "r");
    while (fgets (tempAddress, 1000, ifp) != NULL) {
        fscanf(ifp, "%[^\n]", tempAddress);
        const char *backing_filename = argv[1];
        int backing_fd = open(backing_filename, O_RDONLY);
       if (backing = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, backing_fd, 0)) {
           x++;
       }

        //open simulation of address bus and read the first line
        logical_address = atoi(tempAddress);

        //extract low order 8 bits from the logical_address.
        offset = logical_address & OFFSET_MASK;

        //extract bits 8 through 15. This is the page number gotten by shifting right 8 bits
        logical_page = (logical_address >> OFFSET_BITS) & PAGE_MASK;

        //When you write the program, the physical page will increment by 1 for each copy
        //from simulated backing store to main memory
        physical_page = x%256;

        //copy from secondary storage to simulated RAM. The address on secondary storage
        //is an offset into backing, computed by multiplying the logical
        //page number by 256 and adding the offset
        memcpy(main_memory + physical_page * PAGE_SIZE,
        backing + logical_page * PAGE_SIZE, PAGE_SIZE);

        //Shift the physical page left 8 bits and or with the offset
        //This has the effect of adding the offset to the physical_page
        physical_address = (physical_page << OFFSET_BITS) | offset;

        //extract the value stored at offset bytes within the page
        signed char value = main_memory[physical_page * PAGE_SIZE + offset];

        printf("Virtual address: %d Physical address: %d Value: %d\n", logical_address, physical_address, value);
    }
    printf("Number of Translated Addresses: %i\n", x);
    printf("Page Faults: %i\n", numPageFaults);
    printf("Page Fault Rate: %d\n", numPageFaults/numPageRefs);
    printf("TLB Hits: %i\n", numTLBHits);
    printf("TLB Hit Rate: %d\n", numTLBHits/numTLBRefs);

    fclose(ifp);


  return 0;
}
