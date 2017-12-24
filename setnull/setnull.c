#include <stdio.h>
#include <stdlib.h>
int hscore[11]={0,0,0,0,0,0,0,0,0,0,0};
int main()
{
    char choose;
    printf("对分数清零？(y/n):");
    scanf("%c",&choose);
    if(choose == 'Y' || choose == 'y')
    {
        DSave();
         printf("success");
    }
    return 0;
}
