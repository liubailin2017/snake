#include<stdio.h>
#include"file.h"
extern int hscore[11];
int DSave(){
    char s[] = "���ļ����������߷�����,�벻Ҫ�޸ģ���������쳣������setnull.exe";
    FILE *save = fopen("hscore.sav","w");
    fwrite(hscore,sizeof(int),10,save);
    fwrite(s,sizeof(s),1,save);
    fclose(save);}

int DRead(){
    FILE* fload = fopen("hscore.sav","r");
    if(fload == NULL) exit(1);
    fread(hscore,sizeof(int),10,fload);
    fclose (fload);}
