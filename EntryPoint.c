// Shubham Girish Dhirde

#include "MyHeader.h"

extern UFDT UFDTArr[MAXINODE];		//One Process can open MAX_UFDT
extern SUPERBLOCK SUPERBOLCKobj;
extern PINODE head;

int main()
{
    int fd = 0;
    int ret = 0;
    int count = 0;
    static int rdOffset = 0;

    char * ptr =NULL;

    char str[80];
    char arr[MAXFILESIZE];
    char command[4][80];

    InitialiseSuperBlock();
    printf("CDFS Shell :> SuperBlock initialised successfully\n");

    CreateDILB();
    printf("CDFS Shell :> DILB created successfully\n");

    while(1)
    {
        fflush(stdin);
        fflush(stdout);
        strcpy(str,"");

        printf("CDFS Shell :> ");
        fgets(str,80,stdin);

        count=sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);

        if(1 == count)
        {
            if(strcmp(command[0],"ls")==0)
            {
                ls_file();
            }
            else if(strcmp(command[0],"closeall")==0)
            {
                closeall();
                printf("Files closed succesfully\n");
            }
            else if(strcmp(command[0],"clear")==0)
            {
                system("clear");
                continue;
            }
            else if(strcmp(command[0],"help")==0)
            {
                DisplayHelp();
                continue;
            }
            else if(strcmp(command[0],"exit")==0)
            {
                exitX();
                printf("Terminating CDFS Shell\n");
                break;
            }
            else
            {
                printf("\nError : Command not found!!!\n");
                continue;
            }
        }                                                   
        else if(2 == count)
        {
            if(strcmp(command[0],"stat")==0)
            {
                ret = stat_file(command[1]);
                if(-1 == ret)
                {
                    printf("Error : Incorrect parameters!!!\n");
                }
                if(-2 == ret)
                {
                    printf("Error : There is no such file!!!\n");
                }
                if(0 == ret){}
                continue;
            }
            else if(strcmp(command[0],"fstat")==0)
            {
                ret = fstat_file(atoi(command[1]));
                if(-1 == ret)
                {
                    printf("Error : There is no such file!!!\n");
                }
                if(0 == ret){}
                continue;
            }
            else if(strcmp(command[0],"close")==0)
            {
                ret = close_file(command[1]);

                if(-1 == ret)
                {
                    printf("Error : There is no such file!!!\n");
                }
                else if(ret >= 0)
                {
                    printf("File closed succesfully\n");
                }
                continue;
            }
            else if(strcmp(command[0],"rm")==0)
            {
                ret=rm_file(command[1]);

                if(-1 == ret)
                    printf("Error : There is no such file!!!\n");
                else if(0 == ret)
                    printf("File removed  successfully\n");
            }
            else if(strcmp(command[0],"truncate")==0)
            {
                ret = truncate_file(command[1],(MAXFILESIZE/20));

                if(-1 == ret)
                {
                    printf("Error : There is no such file!!!\n");
                }
                else if(-2 == ret)
                {
                    printf("Error : Incorrect parameters!!!\n");
                }
                else if(-4 == ret)
                {
                    printf("Error : File is being used, close all instance to modify!!!\n");
                }
                else if(ret > 0)
                {
                    printf("File succesfully truncated\n");
                }
                continue;
            }
            else if(strcmp(command[0],"man")==0)
            {
               man(command[1]);
            }
            else if(strcmp(command[0],"write")==0)
            {
                fd = GetFDFromName(command[1]);
                if(fd == -1)
                {
                    printf("Error : No such file in directory!!!\n");
                    continue;
                }
                
                printf("Enter data\n");
                scanf("%[^\n]",arr);

                ret = strlen(arr);
                if(0 == ret)
                {
                    printf("Error : Incorrect parameters!!!\n");
                    continue;
                }
                
                ret=write_file(fd,arr,ret);

                if(-1 == ret)
                {
                    printf("Error : Permission denied!!!\n");
                }
                else if(-2 == ret)
                {
                    printf("Error : No sufficent space availale in file to write!!!\n");
                }
                else if(-3 == ret)
                {
                    printf("Error : It is not regular file!!!\n");
                }
                continue;
            }
            else if(strcmp(command[0],"read")==0)
            {
                ret = read_file(command[1],arr,MAXFILESIZE);

                 if(-1 == ret)
                {
                    printf("Error : There is no such file!!!\n");
                }
                else if(-2 == ret)
                {
                    printf("Error : Permission denied!!!\n");
                }
                else if(-3 == ret)
                {
                    printf("Error : Incorrect parameters!!!\n");
                }
                else if(-4 == ret)
                {
                    printf("Error : File is empty!!!\n");
                }
                else if(-5 == ret)
                {
                    printf("Error : Access denied!!!\n");
                }
                else if(0 == ret)
                {
                    printf("\n%s\n",arr);
                }
                continue;
            }
            else if(strcmp(command[0],"create")==0)
            {
                ret = ext_varify(command[1]);
                if(-1 == ret)
                {
                    printf("Error : File type not supported!!!\n");
                }
                else
                {
                    ret = CreateFile(command[1],READ + WRITE);

                    if(ret >=0)
                    {
                        printf("File is succesfully created with file descriptor : %d\n",ret);
                    }
                    if(-1 == ret)
                        printf("Error : Incorrect parameters!!!\n");
                    if(-2 == ret)
                        printf("Error : There is no free inode!!!\n");
                    if(-3 == ret)
                        printf("Error : File already exist!!!\n");
                    if(-4 == ret)
                        printf("Error : Memory allocation failure!!!\n");
                    continue;
                }
            }
            else if(strcmp(command[0],"open")==0)
            {
                ret = open_file(command[1],READ+WRITE);

                if(-1 == ret)
                {
                    printf("Error : Incorrect Parameters!!!\n");
                }
                else if(-2 == ret)
                {
                    printf("Error : Permission denied!!!\n");
                }
                else if(-3 == ret)
                {
                    printf("Error : Could not open file!!!\n");
                }
                continue;
            }
            else
            {
                printf("\nError : Command not found!!!\n");
                continue;
            }
        }                                                   
        else if(3 == count)
        {
            if(strcmp(command[0],"create")==0)
            {
                ret = CreateFile(command[1],atoi(command[2]));

                if(ret >=0)
                {
                    printf("File is succesfully created with file descriptor : %d\n",ret);
                }
                if(-1 == ret)
                    printf("Error : Incorrect Parameters!!!\n");
                if(-2 == ret)
                    printf("Error : There is no free inode!!!\n");
                if(-3 == ret)
                    printf("Error : File already exist!!!\n");
                if(-4 == ret)
                    printf("Error : Memory allocation failure!!!\n");
                continue;
            }
            else if(strcmp(command[0],"open")==0)
            {
                if(strcasecmp(command[2],"r"))
                {
                    ret = open_file(command[1],READ);
                }
                else if(strcasecmp(command[2],"w"))
                {
                    ret = open_file(command[1],WRITE);
                }
                else if(strcasecmp(command[2],"rw"))
                {
                    ret = open_file(command[1],READ+WRITE);
                }

                if(-1 == ret)
                {
                    printf("Error : There is no such file!!!\n");
                }
                else if(-2 == ret)
                {
                    printf("Error : No space availale in file!!!\n");
                }
                else if(-3 == ret)
                {
                    printf("Error : Permission denied!!!\n");
                }
                else if(-4 == ret)
                {
                    printf("Error : Access denied!!!\n");   
                }
                continue;
            }
            else if(strcmp(command[0],"truncate")==0)
            {
                ret = truncate_file(command[1],atoi(command[2]));

                if(-1 == ret)
                {
                    printf("Error : There is no such file!!!\n");
                }
                else if(-2 == ret)
                {
                    printf("Error : Incorrect parameters!!!\n");
                }
                else if(-4 == ret)
                {
                    printf("Error : File is being used, close all instance to modify!!!\n");
                }
                else if(ret > 0)
                {
                    printf("File succesfully truncated\n");
                }
                continue;
            }
            else if(strcmp(command[0],"read")==0)
            {
                ret = read_file(command[1],arr,atoi(command[2]));

                 if(-1 == ret)
                {
                    printf("Error : There is no such file!!!\n");
                }
                else if(-2 == ret)
                {
                    printf("Error : Permission denied!!!\n");
                }
                else if(-3 == ret)
                {
                    printf("Error : Incorrect parameters!!!\n");
                }
                else if(-4 == ret)
                {
                    printf("Error : File is empty!!!\n");
                }
                else if(-5 == ret)
                {
                    printf("Error : Access denied!!!\n");
                }
                else if(0 == ret)
                {
                    printf("%s\n",arr);
                }
                continue;
            }
            else
            {
                printf("\nError : Command not found!!!\n");
                continue;
            }
        }                                                       
        else if(4 == count)
        {
           if(strcmp(command[0],"lseek")==0)
           {
               fd = GetFDFromName(command[1]);
               if(-1 == fd)
               {
                   printf("Error : Incorrect parameters!!!\n");
                   continue;
               }

               ret = LseekFile(fd,atoi(command[2]),atoi(command[3]));
               if(-1 == ret)
               {
                   printf("ERROR : Enable to perform lseek!!!\n");
               }
           }
           else
            {
                printf("\nError : Command not found!!!\n");
                continue;
            }
        }                                                       
    }                                                           
    return 0;
}
