#include <stdio.h>
#include <stdlib.h>
static int hscore[11]={0,0,0,0,0,0,0,0,0,0,0};
int bk_main()
{
    char choose;
    printf("清零(y/n):");
    scanf("%c",&choose);
    if(choose == 'Y' || choose == 'y')
    {
        DSave();
        printf("success");
    }
    return 0;
}
