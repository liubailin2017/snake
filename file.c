#include<stdio.h>
#include"file.h"
extern int hscore[11];
int DSave(){
    char s[] = "此文件保存的是最高分数据,请不要修改！如果程序异常请运行setnull.exe";
    FILE *save = fopen("hscore.sav","w");
    fwrite(hscore,sizeof(int),10,save);
    fwrite(s,sizeof(s),1,save);
    fclose(save);}

int DRead(){
    FILE* fload = fopen("hscore.sav","r");
    if(fload == NULL) exit(1);
    fread(hscore,sizeof(int),10,fload);
    fclose (fload);}
