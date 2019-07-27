// Shubham Girish Dhirde

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<iostream>

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 1024

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

typedef struct superblock
{
    int TotalInodes;
    int FreeInodes;
}SUPERBLOCK, *PSUPERBLOCK;

typedef struct inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int FileActualSize;
    int FileType;

    char * Buffer;
    struct inode * next;

    int LinkCount;
    int ReferenceCount;
    int Permission;
}INODE, *PINODE, **PPINODE;

typedef struct filetable
{
    int readoffset;
    int writeoffset;
    int count;
    int mode;
    PINODE ptrinode;
}FILETABLE, *PFILETABLE;

typedef struct udft
{
    PFILETABLE ptrfiletable;
}UFDT;

void CreateDILB();
void InitialiseSuperBlock();

void exitX();
void ls_file();
void closeall();
void man(char *);
void DisplayHelp();

PINODE Get_Inode(char *);
int GetFDFromName(char *);

int fstat_file(int);
int rm_file(char *);
int stat_file(char *);
int close_file(char *);
int open_file(char *,int);
int CreateFile(char *, int);
int LseekFile(int, int, int);
int truncate_file(char *,int);
int write_file(int,char *,int);
int read_file(char *,char *,int);

int ext_varify(char *);

extern UFDT UFDTArr[MAXINODE];
extern SUPERBLOCK SUPERBLOCKobj;
extern PINODE head;