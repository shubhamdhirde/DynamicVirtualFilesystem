// Shubham Girish Dhirde

#include "MyHeader.h"

UFDT UFDTArr[MAXINODE];		//One Process can open MAX_UFDT
SUPERBLOCK SUPERBOLCKobj;
PINODE head = NULL;

///////////////////////////////////////////////////////////////////////////////
//
//  Name :          CreateDILB
//  INPUT :         NONE
//  OUTPUT :        NONE
//  Description :   This fucntion is used to create Disk Inode List Block
//
////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i=1;
    PINODE temp=head;

    while(i<=MAXINODE)
    {
        PINODE newn = (PINODE)malloc(sizeof(INODE));

        newn->FileName;
        newn->InodeNumber=i;
        newn->FileSize=0;
        newn->FileActualSize=0;
        newn->FileType=0;

        newn->Buffer=NULL;
        newn->next=NULL;

        newn->LinkCount=0;
        newn->ReferenceCount=0;
        newn->Permission=0;

        if(NULL == temp)
        {
            temp=newn;
            head=temp;
        }
        else
        {
            while(NULL != temp->next)
            {
                temp = temp->next;
            }
            temp->next=newn;
        }
        i++;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name :          InitialiseSuperBlock
//  INPUT :         NONE
//  OUTPUT :        NONE
//  Description :   This fucntion is used to initialise super block and UFDT
//
///////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    int i=0;

    for(i=0;i<MAXINODE;i++)
    {
        UFDTArr[i].ptrfiletable=NULL;
    }
    
    SUPERBOLCKobj.TotalInodes=MAXINODE;
    SUPERBOLCKobj.FreeInodes=MAXINODE;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :              man
//  INPUT:              NONE
//  OUTPUT :            NONE
//  Description :       This function is used to display manual of commands used
//
///////////////////////////////////////////////////////////////////////////////////////////

void man(char *name)
{
    if(name == NULL) 
        return;
    
    if(strcmp(name,"create") == 0)
    {
        printf("Description : Used to create new regular file\n");
        printf("Usage : create File_name[.txt]|[.docx] Permission[default = 3]\n");
    }
    else if(strcmp(name,"read") == 0)
    {
        printf("Description : Used to read from regular file\n");
        printf("Usage : read File_name readOffset[default = 0]\n");
    }
    else if(strcmp(name,"write") == 0)
    {
        printf("Description : Used to write into regular file\n");
        printf("Usage : write File_name writeOffset\n");
    }
    else if(strcmp(name,"ls") == 0)
    {
        printf("Description : Used to list files\n");
        printf("Usage : ls\n");
    }
    else if(strcmp(name,"stat") == 0)
    {
        printf("Description : Used to display information of file\n");
        printf("Usage : stat File_name\n");
    }
    else if(strcmp(name,"fstat") == 0)
    {
        printf("Description : Used to display information of file from file descriptor\n");
        printf("Usage : fstat File_descriptor\n");
    }
    else if(strcmp(name,"truncate") == 0)
    {
        printf("Description : Used to shrink file size\n");
        printf("Usage : truncate File_name File_size\n");
    }
    else if(strcmp(name,"open") == 0)
    {
        printf("Description : Used to open file from file descriptor\n");
        printf("Usage : open File_name Mode[r/w/r+w]\n");
    }
    else if(strcmp(name,"close") == 0)
    {
        printf("Description : Used to close files\n");
        printf("Usage : close File_name\n");
    }
    else if(strcmp(name,"closeall") == 0)
    {
        printf("Description : Used to close all open filesS\n");
        printf("Usage : closeall\n");
    }
    else if(strcmp(name,"rm") == 0)
    {
        printf("Description : Used to remove/delete file from file descriptor\n");
        printf("Usage : stat File_name\n");
    }
    else if(strcmp(name,"lseek") == 0)
    {
        printf("Description : Used to change file offset\n");
        printf("Usage : lseek File_Name ChangeInOffset StartPoint\n");
    }
    else
    {
        printf("ERROR : No manual entry available.\n");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :              DisplayHelp
//  INPUT:              NONE
//  OUTPUT :            NONE
//  Description :       This function is used to display system calls used in project
//
///////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("ls : To List out all files\n");
    printf("clear : To clear console\n");
    printf("open : To open the file\n");
    printf("create : To create text document file");
    printf("close : To close the file descriptor\n");
    printf("closeall : To close all the file descriptor\n");
    printf("read : To read from file descriptor\n");
    printf("write :To write into file descriptor\n");
    printf("exit : To exit from DCFS");
    printf("stat : To printf information about a file\n");
    printf("fstat :fstat :Used to display information of file from file descriptor\n");
    printf("truncate : To truncate size of file by bytes\n");
    printf("rm : To remove file\n");
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :              GetFDFromName
//  INPUT:              CHARACTER POINTER
//  OUTPUT :            INTEGER
//  Description :       This function is used to get file descriptor from file name
//
///////////////////////////////////////////////////////////////////////////////////////////

int GetFDFromName(char *name)
{
    int i = 0;
    
    while(i<MAXINODE)
    {
        if(UFDTArr[i].ptrfiletable != NULL)
            if(strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName),name)==0)
                break;
        i++;
    }
    if(i == MAXINODE) 
        return -1;
    else 
        return i;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :              Get_Inode
//  INPUT:              CHARACTER POINTER
//  OUTPUT :            STRUCTURE POINTER
//  Description :       This function is used to check if file with same name is already in
//                      file system or not
//
///////////////////////////////////////////////////////////////////////////////////////////

PINODE Get_Inode(char * name)
{
    PINODE temp1 = head;
    int i = 0;

    if(NULL == name)
        return NULL;

    while(temp1 != NULL)
    {
        if(strcasecmp(name,temp1->FileName)==0)
            break;
        temp1=temp1->next;
    }
    return temp1;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Name :          CreateFile
//  Input :         CHARACTER POINTER, INTEGER
//  Output :        INTEGER
//  Description :   This function is used to create file
//
////////////////////////////////////////////////////////////////////////////////

int CreateFile(char * name, int permission)
{
    int i = 1;
    int j = 0;
    PINODE temp2 = head;

    if(name==NULL || permission<0 || permission>3 )
	{
		return -1;
	}

    if(SUPERBOLCKobj.FreeInodes == 0)
    {
        return -2;
    }

    if(Get_Inode(name) != NULL)
    {
        return -3;
    }

    while(temp2 != NULL)
    {
        if(temp2->FileType == 0)
        {
            break;
        }
        temp2=temp2->next;
    }

    while(j < MAXINODE)
    {
        if(UFDTArr[j].ptrfiletable == NULL)
            break;
        else
            j++;
    }

    UFDTArr[j].ptrfiletable=(PFILETABLE)malloc(sizeof(FILETABLE));
    
    if(NULL == UFDTArr[j].ptrfiletable)
    {
        return -4;
    }
    UFDTArr[j].ptrfiletable->ptrinode=temp2;

    UFDTArr[j].ptrfiletable->ptrinode->Buffer=(char*)malloc(sizeof(MAXFILESIZE));

    if(NULL == UFDTArr[j].ptrfiletable->ptrinode->Buffer)
    {
        free(UFDTArr[j].ptrfiletable);
        UFDTArr[j].ptrfiletable=NULL;
        return -4;
    }

    UFDTArr[j].ptrfiletable->count=1;               
    UFDTArr[j].ptrfiletable->readoffset=0;
    UFDTArr[j].ptrfiletable->writeoffset=0;
    UFDTArr[j].ptrfiletable->mode=permission;

    UFDTArr[j].ptrfiletable->ptrinode->FileSize=MAXFILESIZE;
    UFDTArr[j].ptrfiletable->ptrinode->FileType=REGULAR;
    UFDTArr[j].ptrfiletable->ptrinode->LinkCount=1;
    UFDTArr[j].ptrfiletable->ptrinode->ReferenceCount=1;
    UFDTArr[j].ptrfiletable->ptrinode->Permission=permission;
    std::copy(name,name+20,UFDTArr[j].ptrfiletable->ptrinode->FileName);
    
    SUPERBOLCKobj.FreeInodes--;
    
    return j;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          ls_file
//  Input :         NONE
//  Output :        NONE
//  Description :   This function is used display files from DILB
//
/////////////////////////////////////////////////////////////////////////////////////////

void ls_file()
{
    PINODE temp = head;

    while(temp != NULL)
    {
        if(temp->Buffer != NULL )
        {
            printf("%s\n",temp->FileName);
        }
        temp=temp->next;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          rm_file
//  Input :         NONE
//  Output :        NONE
//  Description :   This function is used to delete/remove file from file from 
//                  file descriptor
//
/////////////////////////////////////////////////////////////////////////////////////////

int rm_file(char * name)
{
    int i=GetFDFromName(name);

    if(-1 == i)
    {
        return -1;
    }
    else if(i >= 0)
    {
        if(UFDTArr[i].ptrfiletable->ptrinode->FileType != 0)
        {
            free(UFDTArr[i].ptrfiletable->ptrinode->Buffer);
            UFDTArr[i].ptrfiletable->ptrinode->Buffer=NULL;
            UFDTArr[i].ptrfiletable->ptrinode->LinkCount=0;
            UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount=0;
            UFDTArr[i].ptrfiletable->ptrinode->FileType=0;
            memset(UFDTArr[i].ptrfiletable->ptrinode->FileName + 0,'\0',50*sizeof(char));

            free(UFDTArr[i].ptrfiletable);
            UFDTArr[i].ptrfiletable=NULL;

            (SUPERBOLCKobj.FreeInodes)++;
        }
        return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          exitX
//  Input :         NONE
//  Output :        NONE
//  Description :   This function is used to deallocate resources before terminating
//
/////////////////////////////////////////////////////////////////////////////////////////

void exitX()
{
    int i = 0;
    PINODE temp = head;

    for(i=0;i<MAXINODE;i++)
    {
        if(UFDTArr[i].ptrfiletable != NULL);
        {
            free(UFDTArr[i].ptrfiletable);
            UFDTArr[i].ptrfiletable=NULL;
        }
    }

    i=1;
    while(i<=MAXINODE)
    {
        if(temp->Buffer != NULL)
        {
            free(temp->Buffer);
        }
        temp=temp->next;
        i++;
    }

    temp=head;

    while(NULL != head)
    {
        head=head->next;
        free(temp);
        temp=head;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          stat_file
//  Input :         CHARACTER POINTER
//  Output :        INTEGER
//  Description :   This function is used display statastical information about file
//
/////////////////////////////////////////////////////////////////////////////////////////

int stat_file(char *name)
{
    int i = GetFDFromName(name);

    if(NULL == name)
    {
        return -1;
    }
    if(-1 == i)
    {
        return -2;
    }
    else if(i >= 0)
    {   
        printf("------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| File Name\t| Inode Nuber\t| File Size\t| Actual File Size\t| File Type\t| Reference Count | Link Count\t| Permission \n");
        printf("------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %s\t| %d\t\t| %d\t\t| %d\t\t\t| %d\t\t| %d\t\t  | %d\t\t| %d\t    \n"
            ,UFDTArr[i].ptrfiletable->ptrinode->FileName
                ,UFDTArr[i].ptrfiletable->ptrinode->InodeNumber
                ,UFDTArr[i].ptrfiletable->ptrinode->FileSize,UFDTArr[i].ptrfiletable->ptrinode->FileActualSize
                ,UFDTArr[i].ptrfiletable->ptrinode->FileType,UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount
                ,UFDTArr[i].ptrfiletable->ptrinode->LinkCount,UFDTArr[i].ptrfiletable->ptrinode->Permission);
        printf("------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          fstat_file
//  Input :         CHARACTER POINTER
//  Output :        INTEGER
//  Description :   This function is used display statastical information about file
//                  from file descriptor
//
/////////////////////////////////////////////////////////////////////////////////////////

int fstat_file(int fd)
{
    int i=0;
    
    for(i = 0; i<MAXINODE;i++)
    {
        if((UFDTArr[fd].ptrfiletable == NULL)||(fd > MAXINODE))
        {
            return -1;
        }
        else
        {
            printf("------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("| File Name\t| Inode Nuber\t| File Size\t| Actual File Size\t| File Type\t| Reference Count | Link Count\t| Permission \n");
            printf("------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("| %s\t| %d\t\t| %d\t\t| %d\t\t\t| %d\t\t| %d\t\t  | %d\t\t| %d\t    \n"
                    ,UFDTArr[fd].ptrfiletable->ptrinode->FileName
                    ,UFDTArr[fd].ptrfiletable->ptrinode->InodeNumber
                    ,UFDTArr[fd].ptrfiletable->ptrinode->FileSize,UFDTArr[i].ptrfiletable->ptrinode->FileActualSize
                    ,UFDTArr[fd].ptrfiletable->ptrinode->FileType,UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount
                    ,UFDTArr[fd].ptrfiletable->ptrinode->LinkCount,UFDTArr[i].ptrfiletable->ptrinode->Permission);
            printf("------------------------------------------------------------------------------------------------------------------------------------\n");

            return 0;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          ext_varify
//  Input :         CHARACTER POINTER
//  Output :        INTEGER
//  Description :   This function is used to varify file extention
//
/////////////////////////////////////////////////////////////////////////////////////////

int ext_varify(char * arr)
 {
    char brr[80]={'\0'};
    char crr[4]={'\0'};

    sscanf(arr,"%[^.]%s",brr,crr);

    if(strcasecmp(".txt",crr)== 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
 }

//////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          open_file
//  Input :         CHARACTER POINTER
//  Output :        INTEGER
//  Description :   This function is used to open file from file descriptor
//
/////////////////////////////////////////////////////////////////////////////////////////

int open_file(char * name,int fmode)
{
    int i =0;
    PINODE temp=NULL;

    if(NULL == name)
    {
        return -1;
    }
    temp = Get_Inode(name);
    if(NULL == temp)
    {
        i=CreateFile(name,fmode);
        temp = Get_Inode(name);
        
        if(i >= 0)
        {
            (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

            return 1;
        }
    }
    else
    {
        if(temp->Permission<fmode)
        {
            return -2;
        }
        i=0;

        while(i<MAXINODE)
        {
            if(UFDTArr[i].ptrfiletable == NULL)
            {
                break;
            }
            i++;
        }

        UFDTArr[i].ptrfiletable=(PFILETABLE)malloc(sizeof(FILETABLE));
        if(UFDTArr[i].ptrfiletable == NULL)
        {
            return -3;
        }

        UFDTArr[i].ptrfiletable->count=1;
        UFDTArr[i].ptrfiletable->mode=fmode;

        if(fmode == READ)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
        }
        else if(fmode == WRITE)
        {
            UFDTArr[i].ptrfiletable->writeoffset = 0;
        }
        else if(fmode == READ+WRITE)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;
        }

        UFDTArr[i].ptrfiletable->ptrinode=temp;
        (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

        return i;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          write_file
//  Input :         CHARACTER POINTER
//  Output :        INTEGER
//  Description :   This function is used to write data into the file
//
/////////////////////////////////////////////////////////////////////////////////////////

int write_file(int fd,char*arr,int iSize)
{
    if(((UFDTArr[fd].ptrfiletable->mode) != WRITE) && ((UFDTArr[fd].ptrfiletable->mode) != READ+WRITE))
        return -1;

    if(((UFDTArr[fd].ptrfiletable->ptrinode->Permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->Permission) != READ+WRITE))
        return -1;    

    if((UFDTArr[fd].ptrfiletable->writeoffset) == MAXFILESIZE)
        return-2;

    if((UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR)
        return -3;

    if(UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount == 0)
        (UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)++;    

    strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset),arr,iSize);

    (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + iSize;

    (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + iSize;

    return iSize;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          read_file
//  Input :         CHARACTER POINTER
//  Output :        INTEGER
//  Description :   This function is used to read data from file
//
/////////////////////////////////////////////////////////////////////////////////////////

int read_file(char *name,char *arr,int bytes)
{
    int i = GetFDFromName(name);

    if(-1 == i)
    {
        return -1;
    }
    
    if((UFDTArr[i].ptrfiletable->ptrinode->Permission == WRITE)&&
        (UFDTArr[i].ptrfiletable->ptrinode->Permission != WRITE+READ))
    {
        return -2;
    }
    
    if((UFDTArr[i].ptrfiletable->mode == WRITE)&&
        UFDTArr[i].ptrfiletable->mode != READ+WRITE)
    {
        return -2;
    }
    else if((bytes <= 0))
    {
        return -3;
    }
    else if(UFDTArr[i].ptrfiletable->ptrinode->FileActualSize == 0)
    {
        return -4;
    }
    else if(UFDTArr[i].ptrfiletable->mode == WRITE)
    {
        return -5;
    }

    if(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount == 0)
        (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

    if((bytes > UFDTArr[i].ptrfiletable->ptrinode->FileActualSize))
    {
        memset(arr + 0,'\0',MAXFILESIZE*sizeof(char));

        strncpy(arr,UFDTArr[i].ptrfiletable->ptrinode->Buffer + UFDTArr[i].ptrfiletable->readoffset,UFDTArr[i].ptrfiletable->ptrinode->FileActualSize);

        UFDTArr[i].ptrfiletable->readoffset=0;

        return 0;
    }
    else
    {
        memset(arr + 0,'\0',MAXFILESIZE*sizeof(char));
        
        strncpy(arr,UFDTArr[i].ptrfiletable->ptrinode->Buffer + UFDTArr[i].ptrfiletable->readoffset,bytes);
        UFDTArr[i].ptrfiletable->readoffset = UFDTArr[i].ptrfiletable->readoffset + bytes;

        if(UFDTArr[i].ptrfiletable->readoffset >= UFDTArr[i].ptrfiletable->ptrinode->FileActualSize)
        {
            if(bytes == UFDTArr[i].ptrfiletable->ptrinode->FileActualSize)
            {
                bytes++;
            }
            UFDTArr[i].ptrfiletable->readoffset = 0;
            read_file(name,arr,bytes);
        }
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          close_file
//  Input :         CHARACTER POINTER
//  Output :        INTEGER
//  Description :   This function is used to close file from file descriptor
//
/////////////////////////////////////////////////////////////////////////////////////////

int close_file(char *name)
{
    int i = GetFDFromName(name);

    if(-1 == i)
    {
        return i;
    }
    else if(i >= 0)
    {
        if(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount > 0)
        {   
            (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;

        }
        return i;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          closeall
//  Input :         NONE
//  Output :        INTEGER
//  Description :   This function is used to close all the files from file descriptor
//
/////////////////////////////////////////////////////////////////////////////////////////

void closeall()
{
    int i = 0;

    while(i < 50)
    {
        if(UFDTArr[i].ptrfiletable != NULL)
        {
            UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 0;
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;
        }
        i++;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          truncate_file
//  Input :         CHARACTER POINTER, INTEGER
//  Output :        INTEGER
//  Description :   This function is used to trim size of file by bytes given by user
//  
/////////////////////////////////////////////////////////////////////////////////////////

int truncate_file(char *name,int bytes)
{
    int len = 0;
    char ch='0';

    if(bytes < 0)
    {
        return -2;
    }

    int i = GetFDFromName(name);
    if(-1 == i)
    {
        return i;
    }

    if((bytes > MAXFILESIZE)||(0 == bytes))
    {
        memset(UFDTArr[i].ptrfiletable->ptrinode->Buffer + 0,'\0',MAXFILESIZE*sizeof(char));
        UFDTArr[i].ptrfiletable->readoffset = 0;
        UFDTArr[i].ptrfiletable->writeoffset = 0;

        return 1;
    }
    else
    {
        if(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount == 0)
        {
            if(bytes <= UFDTArr[i].ptrfiletable->ptrinode->FileActualSize)
            {
                printf("Warning : Possible data loss, Enter (y/n)\n");
                scanf("%c",&ch);
                while(1)
                {
                    if((ch == 'y')||(ch == 'Y'))
                    {
                        UFDTArr[i].ptrfiletable->ptrinode->FileSize = bytes;
                        UFDTArr[i].ptrfiletable->ptrinode->FileActualSize=bytes;
                        
                        return 1;
                    }
                    else if((ch == 'n')||(ch == 'N'))
                    {
                        return -3;
                    }
                }
            }
            else
            {
                UFDTArr[i].ptrfiletable->ptrinode->FileSize = bytes;

                return 1;
            }
        }
        else
        {
            return -4;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
//  Name :          LseekFile
//  Input :         INTEGER, INTEGER, INTEGER
//  Output :        INTEGER
//  Description :   This function is used reset offset of a file by offset provided by user
//  
/////////////////////////////////////////////////////////////////////////////////////////////

int LseekFile(int fd, int size, int from)
{
    if((fd < 0)||(from > 2))
        return -1;

    if((UFDTArr[fd].ptrfiletable->mode == READ)|| (UFDTArr[fd].ptrfiletable->mode == READ+WRITE))
    {
        if(from == CURRENT)
        {
            if(((UFDTArr[fd].ptrfiletable->readoffset)+size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
                return -1;
            if(((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0) 
                return -1;
            
            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + size;
        }
        else if(from == START)
        {
            if(size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)) 
                return -1;
            if(size < 0) 
                return -1;

            (UFDTArr[fd].ptrfiletable->readoffset) = size;
        }
        else if(from == END)
        {
            if((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size > MAXFILESIZE)
                return -1;
            if(((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0) 
                return -1;

            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
        }
    }
    else if(UFDTArr[fd].ptrfiletable->mode == WRITE)
    {
        if(from == CURRENT)
        {
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) > MAXFILESIZE) 
                return -1;
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0) 
                return -1;

            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)) 
                (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;

            (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;    
        }
        else if(from == START)
        {
            if(size > MAXFILESIZE) 
                return -1;
            if(size < 0) 
                return -1;

            if(size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
                (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = size;

            (UFDTArr[fd].ptrfiletable->writeoffset) = size;
        }
        else if(from == END)
        {
            if((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size > MAXFILESIZE)
                return -1;
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0) 
                return -1;

            (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
        }    
    }
}
