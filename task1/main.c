#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include "elf.h"

int foreach_phdr(void *map_start, void (*func)(Elf32_Phdr *,int), int arg);
void task1a(int argc, char ** argv);
void printSpecificHeader (Elf32_Phdr *,int);
char * getType(int n);
char * getFlags(int n);


char * getFlags(int n)
{
    int r=0,w=0,e=0;
    char str[100];
    str[0] = '\0';
    if (n/4 == 1)
    {
        r = 1;
    }
    n = n%4;
    if (n/2 == 1)
    {
        w = 1;
    }
    if (n == 1)
    {
        e = 1;
    }
    if (r && w)
    {
        strcat(str, "RW");
    }
    else if (r)
    {
        strcat(str, "R");
    }
    else if (w)
    {
        strcat(str, "W");
    }

    if (r || w)
    {
        strcat(str, " ");
    }
    if (e)
    {
        strcat(str, "E");
    }

    char * s = str;
    return s;
}
char * getType(int n)
{
    switch(n)
    {
        case 0: return "NULL";
        case 1: return "LOAD";
        case 2: return "DYNAMIC";
        case 3: return "INTERP";
        case 4: return "NOTE";
        case 5: return "SHLIB";
        case 6: return "PHDR";
        case 7: return "TLS";
        case 0x65041580: return "PAX_FLAGS";
        case 0x6fffffff: return "HIOS";
        case 0x70000000: return "LOPROC";
        case 0x70000001: return "ARM_EXIDX";
        case 0x7fffffff: return "HIPROC";
        case 0x6474e550: return "GNU_EH_FRAME";
        case 0x6474e551: return "GNU_STACK";
        case 0x6474e552: return "GNU_RELRO";
        default: fprintf(stderr, "Unrecognized type");
            exit(1);
    }
}

int foreach_phdr(void *map_start, void (*func)(Elf32_Phdr *,int), int arg)
{
    Elf32_Ehdr * header = (Elf32_Ehdr *) map_start;
    Elf32_Phdr * phdrTable;
    phdrTable = (Elf32_Phdr *) (map_start + header->e_phoff);
    int i;
    int numberOfHeaders = header->e_phnum;
    printf("Type\t\tOffset\t\tVirtAddr\tPhysAddr\tFileSiz\t\tMemSiz\t\tFlg\tAlign\n");
    for (i=0; i<numberOfHeaders; i++)
    {
        func(phdrTable, i);
    }
    return 0;
}
void printSpecificHeader (Elf32_Phdr * phdrTable, int i)
{
    char * type = getType(phdrTable[i].p_type);
    printf("%s\t", type); // Type
    if (strcmp(type, "GNU_STACK") != 0)
    {
        printf("\t");
    }
    printf("%#08x\t", phdrTable[i].p_offset); // offfset
    printf("%#08x\t", phdrTable[i].p_vaddr); // virtAddr
    printf("%#08x\t", phdrTable[i].p_paddr); // PhysAddr
    printf("%#08x\t", phdrTable[i].p_filesz); // FileSiz
    printf("%#08x\t", phdrTable[i].p_memsz); // MemSiz
    printf("%s\t", getFlags(phdrTable[i].p_flags)); // Flg
    printf("%#03x\t", phdrTable[i].p_align); // Align
    printf("\n");
}
void task1a(int argc, char ** argv)
{
    int fd = -1;
    void *map_start; /* will point to the start of the memory mapped file */
    struct stat fd_stat; /* this is needed to  the size of the file */

    if( (fd = open(argv[1], O_RDWR)) < 0 ) {
        perror("error in open");
        exit(-1);
    }

    if( fstat(fd, &fd_stat) != 0 ) {
        perror("stat failed");
        exit(-1);
    }

    if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0)) == MAP_FAILED ) {
        perror("mmap failed");
        exit(-4);
    }

    foreach_phdr(map_start,printSpecificHeader, 0);

    munmap(map_start, fd_stat.st_size);
}

int main(int argc, char ** argv) {
    task1a(argc, argv);
    return 0;
}
