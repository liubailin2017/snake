#include<stdio.h>
#include<stdio.h>
#include"file.h"
extern int hscore[11];

int DSave(){
    char *home;
    home = getenv("HOME");
    char path[256] ={'\0'};
    strcat(path,home);
    strcat(path,"/hscore.sav");

    char s[] = "this is saving about you high score";

    FILE *save = fopen(path,"w");
    fwrite(hscore,sizeof(int),10,save);
    fwrite(s,sizeof(s),1,save);

    fclose(save);
}

int DRead(){
    char *home;
    home = getenv("HOME");
    char path[256] ={'\0'};
    strcat(path,home);
    strcat(path,"/hscore.sav");
    FILE* fload = fopen(path,"r");
    if(fload == NULL) exit(1);
    fread(hscore,sizeof(int),10,fload);
    fclose (fload);
}
