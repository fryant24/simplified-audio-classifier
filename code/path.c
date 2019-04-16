#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
char re[100][100];
int cot=0;
int readFileList(char *basePath)
{
    DIR *dir;
    struct dirent *ptr;
    char base[1000],tail[10];
    //const char *outfile="path.txt";
    int len,i;

    //FILE *f1;
    //f1 = fopen(outfile, "w");
    if ((dir=opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8){    ///file
            len=strlen(ptr->d_name);
            memset(tail,'\0',sizeof(tail));
            for(i=len-6;i<len;i++){
                tail[i-(len-6)]=ptr->d_name[i];
            }
            if(strcmp(tail,"LR.WAV")==0||strcmp(tail,"LR.wav")==0){
                
                strcpy(re[cot],basePath);
                strcat(re[cot],"/");
                strcat(re[cot],ptr->d_name);
                //fprintf(f1,"%s/%s\n",basePath,ptr->d_name);
                
                printf("d_name:%s/%s\n",basePath,ptr->d_name);
                cot++;
            }
        }
        //else if(ptr->d_type == 10)    ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
        else if(ptr->d_type == 4)    ///dir
        {
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            readFileList(base);
        }
    }
    closedir(dir);
    return 1;
}

int main(void)
{
    DIR *dir;
    char basePath[1000];
    const char *outfile="path.txt";
    FILE *f1=fopen(outfile, "w");
    ///get the current absoulte path
    memset(basePath,'\0',sizeof(basePath));
    getcwd(basePath, 999);
    printf("the current dir is : %s\n",basePath);

    ///get the file list
    memset(basePath,'\0',sizeof(basePath));
    strcpy(basePath,".");
    readFileList(basePath);
    printf("%d\n",cot);
    
    for(int i=0;i<cot;i++){
        /*if windows
        for(int loop=0;loop<strlen(re[i]);loop++){
            if(re[i][loop]=='/')
                re[i][loop]='\\';
        }
        */
        fprintf(f1,"%s\n",re[i]);
    }
    fclose(f1);
    return 0;
}