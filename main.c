/*说明使用的操作系统*/

#define Linux
#include<stdio.h>
#include<stdlib.h>
#include"ver.h"
#include"file.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#define Width 640
#define Height 480

#define DELAY 260
//游戏在窗口左上角坐标
#define XX GameMR.x
#define YY GameMR.y
//游戏在窗口右下角坐标
#define XX2 (GameMR.x + GameMR.w)
#define YY2 (GameMR.y + GameMR.h)
//一节的大小
#define XX3 (GameMR.w)/limitx
#define YY3 (GameMR.h)/limity
#define PASS 0x5201314F
#define SDSize 3
#ifdef Windows
#include<windows.h>
#endif
typedef struct snake
{
    int x ;
    int y ; /*x ,y 不是像素 还是蛇的位置*/
    struct snake *next ;
} Snake ;

typedef struct _DirMsg /*输入队列*/
{
    SDL_Keycode msg[SDSize];
    int Rear;
    int Front;
}DirMsg;
typedef enum _SDirect /*蛇头朝向*/
{
        UP = 1,
        DOWN,
        LEFT,
        RIGHT
}SDirect;

Snake food = {5,0};
Snake *snakeh;//蛇头结点
int Sscore = 0;
int hscore[11] = {0,0,0,0,0,0,0,0,0,0,0};
int isquit = 0 ; // 是否退出
int isgameover = 0;//是否游戏结束
const SDL_Rect GameMR = {20, 40, Width - 40, Height - 60};

int limitx = 0;
int limity = 0;

int Puttext(SDL_Surface *surface, int , int ,int ,SDL_Color ,char *);
int Putpixel(SDL_Surface* , int, int, Uint32);                             //画点
void Bar(SDL_Surface* , SDL_Rect, Uint32);                                 //画框函数
void GameframeR(SDL_Surface* , int); //画游戏框架
void Afood(); //添加食物
void Initsnake();
void Gamerun(SDL_Surface* surface,SDL_Window * window);
void draw(SDL_Surface* ,SDL_Window* ,int);
void Gameover(SDL_Window *);
int isfender();
int isself();
void Smove(SDirect);
void sleft();
void sright();
void sdown();
void sup();
void Inserths(int sc);
void puths(SDL_Surface *surface);
int main(int argc, char *argv[])
{
    Version();

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    DRead();

    SDL_Window* window = SDL_CreateWindow(" My qq:2570667204"
                                          ,SDL_WINDOWPOS_UNDEFINED
                                          ,SDL_WINDOWPOS_UNDEFINED
                                          ,Width,Height
                                          ,SDL_WINDOW_SHOWN);
    SDL_Surface* screen = SDL_GetWindowSurface(window);
//重复游戏 。
     while(!isquit)
    {
        isgameover = 0;
        Initsnake();
        Gamerun(screen,window);
    }


    SDL_FreeSurface(screen);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

/************绘点*****************
 *给一个suface绘一个点
 *Px ,Py 坐标
 *color颜色
 *********************************/
int Putpixel(SDL_Surface *surface, int Px, int Py,Uint32 color)
{
    switch(surface->format->BytesPerPixel)
    {
    case 4 :
    {
        Uint32 *bufp;
        if( Px < surface->pitch/4 && Px >= 0 &&
                Py < surface->h && Py >= 0)
        {
            if(SDL_MUSTLOCK(surface))
                if(SDL_LockSurface(surface) < 0)
                    return 0;
            bufp = (Uint32 *)surface->pixels + Px + Py * surface->pitch/4;
            *bufp = color;
            if(SDL_MUSTLOCK(surface))
            {
                SDL_UnlockSurface(surface);
            }
            return 1 ;    /*  ********** !返回 ************/
        }
        else
            return 0 ;
    }
    break;
    default :
        printf("Unsupported");
    }
    return 0;
}



void Bar(SDL_Surface *surface, SDL_Rect Bar_Rect, Uint32 color)
{
    int barx,bary;
    for (bary = 0; bary < Bar_Rect.h; bary++ )
        for (barx = 0; barx < Bar_Rect.w; barx++ )
        {
            Putpixel(surface, Bar_Rect.x+barx, Bar_Rect.y + bary, color);
        }
}

/************在Surface输出文本************
 *  suface 输出
 *  x,y坐标
 *  大小
 *  fcolor颜色
 *  文本
 *****************************************/
int Puttext(SDL_Surface *surface,
            int x, int y,
            int ptsize,
            SDL_Color fcolor,
            char *text)
{
    SDL_Surface* msg;
    SDL_Rect tpos = {x,y};

    TTF_Font* font = TTF_OpenFont("SDL_font.ttf", ptsize);
    if(font == NULL) printf("ttf Error!");
    msg = TTF_RenderUTF8_Solid(font, text, fcolor);
    SDL_BlitSurface(msg, NULL, surface, &tpos);
    TTF_CloseFont(font);
    SDL_FreeSurface(msg);
    return 1;
}

/**********游戏的窗口框架*******
 *surface
 *分数 /当前分数，当时没设计好。算是一个没用的东西
 ********************************/
void GameframeR(SDL_Surface* surface,int score)
{
    char strscr[]="SCORE :XXXXXXXX Highest : XXXXXXXX";
    SDL_Color  tcolor = {254,254,0};
    SDL_Rect rect1 = {0,30,Width,Height-30};
    SDL_Rect rect2 = {0,0,Width,30};
    SDL_Rect rect3 = {GameMR.x-2,GameMR.y-2,GameMR.w+2,2};
    SDL_Rect rect4 = {GameMR.x-2,GameMR.y,2,GameMR.h};
    SDL_Rect rect5 = {GameMR.x-1,GameMR.y+GameMR.h,GameMR.w+1,1};
    SDL_Rect rect6 = {GameMR.x+GameMR.w,GameMR.y-1,1,GameMR.h+2};
    Bar(surface, rect1, 0x9876543);
    Bar(surface, GameMR,0x87654321);
    Bar(surface, rect3,0x222222);
    Bar(surface, rect4,0x222222);
    Bar(surface, rect5,0x999999);
    Bar(surface, rect6,0x999999);

    Bar(surface,rect2,0x12345678);

    sprintf(strscr,"SCORE :%8d Highest: %8d",score,hscore[0]);
    Puttext(surface, 0, 0, 24, tcolor, strscr);
    static int i = 0;
    i++;
    i = i%4;
    switch(i)
    {
        case 0:
                tcolor.b += 1;
                Puttext(surface,0,50,24,tcolor,"-");
        break;
        case 1:
                tcolor.b += 1;
                Puttext(surface,0,50,24,tcolor,"\\");
                break;
        case 2:
                tcolor.b += 1;
                Puttext(surface,0,50,24,tcolor,"|");
                break;
        case 3:
                tcolor.r -= 100;
                Puttext(surface,0,50,24,tcolor,"/");
                break;


    }
}

/***********更新食物**********
*
***********************************/
void Afood()
{
    Snake* tmp = snakeh;
    int isfoodself = 0; // 食物是否在自己身上
    do
    {
        isfoodself = 0;
        food.x = rand()%limitx;
        food.y = rand()%limity;
        tmp = snakeh;
        while(tmp != NULL)
        {
            if(tmp->x == food.x && tmp->y == food.y )
            {
                isfoodself = 1;
            }
            tmp = tmp->next;

        }
    }
    while(isfoodself == 1);
}
/**********下面三个函数实现只能存2个输入的队列**************/
int IsDrEmpty(DirMsg *sdir)
{
    if(sdir->Front == sdir->Rear)
        return 1;
    else
        return 0;
}
SDL_Keycode PollDirect(DirMsg *sdir)
{
    SDL_Keycode tmp = 0;
    if(!IsDrEmpty(sdir))
    {
        tmp = sdir->msg[sdir->Front];
        sdir->Front ++;
        sdir->Front = sdir->Front % SDSize;
    }
return tmp;
}
int PushNextDirect(DirMsg *sdir ,SDL_Keycode message)
{
    if(sdir->Front != (sdir->Rear + 1)% SDSize)
    {
        sdir->msg[sdir->Rear] = message;
        sdir->Rear++;
        sdir->Rear = sdir->Rear % SDSize;
        return 1;
    }
    else
        return 0;
}
/********游戏运行时*************
 *surface , window
 *游戏运行时，包含事件处理
 *******************************/
void Gamerun(SDL_Surface* surface,SDL_Window * window)
{
    int pause = 1;
    SDL_Event event;
    SDirect sdir = RIGHT;
    DirMsg DirTmp = {{0,0,0},0,0}; //初始化
    while ((!isquit) && (!isgameover))
    {
       while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
             #ifdef Windows
                SendMessage(GetActiveWindow(),WM_NCLBUTTONDOWN, HTCAPTION, 0);
            #endif

                break;

            case SDL_QUIT:
                isquit = 1; //退出
                break;
            case SDL_KEYDOWN:
                PushNextDirect(&DirTmp,event.key.keysym.sym);
                pause = 0;
            }
        }
                switch(PollDirect(&DirTmp))
                {
                case SDLK_DOWN :
                    if (sdir != UP )
                    {
                        sdir = DOWN;
                    }
                    break;
                case SDLK_UP :
                    if(sdir != DOWN)
                    {
                        sdir = UP;
                    }
                    break;
                case SDLK_LEFT :
                    if(sdir != RIGHT)
                    {
                        sdir = LEFT;
                    }
                    break;
                case SDLK_RIGHT :
                    if(sdir != LEFT)
                    {
                        sdir = RIGHT;
                    }
                    break ;
                case SDLK_SPACE :
                    pause = 1;
                }


        if(!pause)
        {
            Smove(sdir);
            SDL_SetWindowTitle(window,"Running...    My qq:2570667204");
        }


        if(Sscore + 3 >= limitx * limity/2)
        {
            limitx = limitx + 5;
            limity = limity + 5;
            printf("limitx,y= %d,%d /n",limitx,limity);
        } // 身体填满了整个窗口，缩小身体

        if(isfender() || isself()) // 游戏结束
        {
            isgameover = 1;
            Gameover(window);

        }
        draw(surface,window,pause);
        SDL_Delay(DELAY);
    }

}

void Initsnake()        //初始化蛇
{
    limitx = 12;
    limity = 9;

    snakeh = (Snake *)malloc(sizeof(Snake));
    snakeh->x = 2;
    snakeh->y = 0;
//添加两节
    Snake* tmp = (Snake *)malloc(sizeof(Snake));
    tmp->x = 1;
    tmp->y = 0;
    snakeh->next = tmp;
    tmp = (Snake *)malloc(sizeof(Snake));
    tmp->x = 0;
    tmp->y = 0;
    tmp->next = NULL ;
    snakeh->next->next = tmp;

}

void draw(SDL_Surface* surface,SDL_Window * window,int ispause)
{
    int nodecolor = 0;
    GameframeR(surface,Sscore);
    Snake* tmp1;
    SDL_Rect rect;
    tmp1 = snakeh;
//画食物
    {
        rect.x = ((food.x) * XX3)+GameMR.x;
        rect.y = ((food.y) * YY3)+GameMR.y;
        rect.w = XX3;
        rect.h = YY3;
        Bar(surface,rect,rand()*0xff & 0xff00ff);
    }
    //画蛇身
    while(tmp1 != NULL)
    {
        rect.x = ((tmp1->x) * XX3+1)+GameMR.x;
        rect.y = ((tmp1->y) * YY3+1)+GameMR.y;
        rect.w = XX3-1;
        rect.h = YY3-1;
        Bar(surface,rect,nodecolor);
        tmp1 = tmp1->next;
        if(nodecolor < 0x00f000) nodecolor +=0x000f00;
    }


    if(ispause) puths(surface);

    SDL_UpdateWindowSurface(window);
}
void Smove(SDirect dir)
{
    Snake* stmp,*stmpn = snakeh;
    Snake NextStep;
    int iEatFood = 0;
    switch(dir)
    {
    case LEFT:
        {
            NextStep.x = snakeh->x - 1;
            NextStep.y = snakeh->y;
        }
        break;
    case UP:
        {
            NextStep.x = snakeh->x;
            NextStep.y = snakeh->y - 1;
        }
        break;
    case DOWN:
        {
            NextStep.x = snakeh->x;
            NextStep.y = snakeh->y + 1;
        }
        break;
    case RIGHT:
        {
            NextStep.x = snakeh->x + 1;
            NextStep.y = snakeh->y;
        }
        break;
    }

    if(NextStep.x == food.x && NextStep.y == food.y )
    {
            stmp = (Snake *)malloc(sizeof(Snake));
            iEatFood = 1;
    }
    else
    {
        while(stmpn->next->next != NULL)
        {
            stmpn = stmpn ->next ;
        }
        stmp = stmpn->next;
        stmpn->next = NULL ;
    }

    switch(dir)
    {
    case LEFT:
        {
            stmp->x = snakeh->x - 1;
            stmp->y = snakeh->y;
        }
        break;
    case UP:
        {
            stmp->x = snakeh->x;
            stmp->y = snakeh->y - 1;
        }
        break;
    case DOWN:
        {
            stmp->x = snakeh->x;
            stmp->y = snakeh->y + 1;
        }
        break;
    case RIGHT:
        {
            stmp->x = snakeh->x + 1;
            stmp->y = snakeh->y ;
        }
        break;
    }
        stmp->next = snakeh;
        snakeh = stmp ;
        if(iEatFood)
        {
            Afood();
            Sscore ++ ;
        }
}

void Gameover(SDL_Window *window)
{
    SDL_SetWindowTitle(window,"game over");
    Snake* tmp ;
    food.x = 5 ;
    food.y = 0 ;
//清除链表
    while(snakeh != NULL )
    {
        if(snakeh != NULL) tmp = snakeh ;
        snakeh = snakeh->next;
        free(tmp);
    }

    Inserths(Sscore);//插入分数
    DSave();
    Sscore = 0 ;
}

int isfender()
{
    if (snakeh->x <= limitx - 1 && snakeh -> x >= 0 )
        if(snakeh->y <= limity - 1 && snakeh->y >= 0 )
            return 0;
    return 1 ;
}
int isself()
{
    int is = 0;
    Snake* tmp = snakeh;
    while(tmp->next != NULL)
    {
        tmp = tmp ->next ;
        if(tmp->x == snakeh->x && tmp->y == snakeh->y) is = 1;
    }
    return is ;
}
void puths(SDL_Surface *surface)
{
    int line;
    char strscr[13];

    SDL_Color  tcolor = {12,128,0};
    for(line = 0 ; line <= 9; line ++)
    {
        sprintf(strscr,"H%2d:%8d",line+1,hscore[line]);
        Puttext(surface,XX, YY+24*line, 24, tcolor, strscr);
    }

}
void Inserths(int sc) //插入最高分
{
    int i ;
    for(i = 9 ; hscore[i] < sc && i>=0 ; i--)
        hscore[i+1]= hscore[i];
    i++;
    hscore[i] = sc;
}
