/*è¯Žæäœ¿çšçæäœç³»ç»*/

#include"title.h"

#define Linux
#include<stdio.h>
#include<stdlib.h>
#include"ver.h"
#include"file.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#define Width 840
#define Height 560
#define DELAY 260
//æžžæåšçªå£å·Šäžè§åæ 
#define XX GameMR.x
#define YY GameMR.y
//æžžæåšçªå£å³äžè§åæ 
#define XX2 (GameMR.x + GameMR.w)
#define YY2 (GameMR.y + GameMR.h)
//äžèçå€§å°
#define XX3 (GameMR.w)/limitx
#define YY3 (GameMR.h)/limity
#define PASS 0x5201314F
#define SDSize 30
#ifdef Windows
#include<windows.h>
#endif
typedef struct snake
{
    int x ;
    int y ; /*x ,y äžæ¯åçŽ  èæ¯èçäœçœ®*/
    struct snake *next ;
} Snake;

typedef struct _DirMsg /*èŸå¥éå*/
{
    SDL_Keycode msg[SDSize];
    int Rear;
    int Front;
}DirMsg;

typedef enum _SDirect /*èå€Žæå*/
{
        UP = 1,
        DOWN,
        LEFT,
        RIGHT
}SDirect;

Snake food = {4,0};
Snake *snakeh;//èå€Žç»ç¹
int Sscore = 0;
int hscore[11] = {0,0,0,0,0,0,0,0,0,0,0};
int isquit = 0 ; // æ¯åŠéåº
int isgameover = 0;//æ¯åŠæžžæç»æ
const SDL_Rect GameMR = {20, 40, Width - 40, Height - 60};

int limitx = 0;
int limity = 0;


static DirMsg DirTmp = {{0},0,0}; //èªå·±ç»Žæ€çäžäžªæ¶æ¯éå
static DirMsg *pDirTmp = &DirTmp;

int Puttext(SDL_Surface *surface, int , int ,int ,SDL_Color ,char *);
int Putpixel(SDL_Surface* , int, int, Uint32);                             //ç»ç¹
void Bar(SDL_Surface* , SDL_Rect, Uint32);                                 //ç»æ¡åœæ°
void GameframeR(SDL_Surface* , int); //ç»æžžææ¡æ¶
void Afood(); //æ·»å é£ç©
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
//éå€æžžæ ã
     while(!isquit)
    {
        isgameover = 0;
        Initsnake();
//        init(window);
//    	startChangeTitle();//开始自动更新标题
        Gamerun(screen,window);
  //      stopChangeTitle();
    }


    SDL_FreeSurface(screen);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

/************ç»ç¹*****************
 *ç»äžäžªsufaceç»äžäžªç¹
 *Px ,Py åæ 
 *coloré¢è²
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
            return 1 ;    /*  ********** !è¿å ************/
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

int swapInt(int *t1, int *t2){
    int t = *t1;
    *t1 = *t2;
    *t2 = t;
}

/**
w æ¯äžªäžåå¥œåŒå»ºè®®äŒ å¥1
**/
#include "stdlib.h"

#include "math.h"



int rounding(const float a){return (int) (a + 0.5);}

void dda(SDL_Surface *surface,int x0, int y0, int xEnd, int yEnd,Uint32 color)

{
    int dx = xEnd - x0, dy = yEnd - y0, steps, k;
    float xInc, yInc, x = (float)x0, y = (float)y0;
    if(fabs(dx)>fabs(dy))
    {
         steps = fabs(dx);
    }
    else
    {
         steps = fabs(dy);
    }

    xInc = (float)(dx)/(float)(steps);
    yInc = (float)(dy)/(float)(steps);
    Putpixel(surface,rounding(x),rounding(y),color);
    for(k=0;k<steps;k++)
    {
         x += xInc;
         y += yInc;
         Putpixel(surface,rounding(x),rounding(y),color);
    }
}
/*
param width is invalid
*/

void line(SDL_Surface *surface,int x,int y ,int x2,int y2,Uint32 color,int width) {
    dda(surface,x,y,x2,y2,color);
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

/************åšSurfaceèŸåºææ¬************
 *  suface èŸåº
 *  x,yåæ 
 *  å€§å°
 *  fcoloré¢è²
 *  ææ¬
 *****************************************/
int Puttext(SDL_Surface *surface,
            int x, int y,
            int ptsize,
            SDL_Color fcolor,
            char *text)
{
    SDL_Surface* msg;
    SDL_Rect tpos = {x,y};

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/SDL_font.ttf", ptsize);
    if(font == NULL) printf("ttf Error!");
    msg = TTF_RenderUTF8_Solid(font, text, fcolor);
    SDL_BlitSurface(msg, NULL, surface, &tpos);
    TTF_CloseFont(font);
    SDL_FreeSurface(msg);
    return 1;
}

/**********æžžæççªå£æ¡æ¶*******
 *surface
 *åæ° /åœååæ°ïŒåœæ¶æ²¡è®Ÿè®¡å¥œãç®æ¯äžäžªæ²¡çšçäžè¥¿
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

/***********æŽæ°é£ç©**********
*
***********************************/
void Afood()
{
    Snake* tmp = snakeh;
    int isfoodself = 0; // é£ç©æ¯åŠåšèªå·±èº«äž
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
/**********äžé¢äžäžªåœæ°å®ç°åªèœå­2äžªèŸå¥çéå**************/
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

/**
    åªçéå€Ž

sdir åŸå€ççæ¶æ¯éå

è¿åïŒ
    éå€Žæ¶æ¯ïŒ åŠæéåäžºç©ºåè¿å0
*/
SDL_Keycode peekDirect(DirMsg *sdir) {
    SDL_Keycode result = 0;
    if(!IsDrEmpty(sdir)) {
        result = sdir ->msg[sdir->Front];
    }
    return result;
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
/********æžžæè¿è¡æ¶*************
 *surface , window
 *æžžæè¿è¡æ¶ïŒåå«äºä»¶å€ç
 *******************************/


void Gamerun(SDL_Surface* surface,SDL_Window * window)
{
    int pause = 1;
    SDL_Event event;
    SDirect sdir = RIGHT;
    SDL_Keycode code;
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
                isquit = 1; //éåº
                break;
            case SDL_KEYDOWN:
                 code=event.key.keysym.sym;
                if(code != peekDirect(pDirTmp))
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
        } // èº«äœå¡«æ»¡äºæŽäžªçªå£ïŒçŒ©å°èº«äœ

        if(isfender() || isself()) // æžžæç»æ
        {
            isgameover = 1;
            Gameover(window);

        }
        draw(surface,window,pause);
        SDL_Delay(DELAY);
    }

}

void Initsnake()        //åå§åè
{
    limitx = 12;
    limity = 9;

    snakeh = (Snake *)malloc(sizeof(Snake));
    snakeh->x = 2;
    snakeh->y = 0;
//æ·»å äž€è
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
//ç»é£ç©
    {
        rect.x = ((food.x) * XX3)+GameMR.x;
        rect.y = ((food.y) * YY3)+GameMR.y;
        rect.w = XX3;
        rect.h = YY3;
        Bar(surface,rect,rand()*0xff & 0xff00ff);
    }
    //ç»èèº«
    SDL_Rect tRect;
    tRect.x = -1;
    tRect.y = -1;
    while(tmp1 != NULL)
    {
        rect.x = ((tmp1->x) * XX3+1)+GameMR.x;
        rect.y = ((tmp1->y) * YY3+1)+GameMR.y;
        rect.w = XX3-1;
        rect.h = YY3-1;
        Bar(surface,rect,nodecolor);
        if(tRect.x != -1 && tRect.y != -1) {
            line(surface,tRect.x+tRect.w/2,tRect.y+tRect.h/2,rect.x+rect.w/2,rect.y+rect.h/2,0x005500,10);
        }
        tRect = rect;

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
	stmp->x = NextStep.x;
	stmp->y = NextStep.y;

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
//æžé€éŸè¡š
    while(snakeh != NULL )
    {
        if(snakeh != NULL) tmp = snakeh ;
        snakeh = snakeh->next;
        free(tmp);
    }

    Inserths(Sscore);//æå¥åæ°
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
void Inserths(int sc) //æå¥æé«å
{
    int i ;
    for(i = 9 ; hscore[i] < sc && i>=0 ; i--)
        hscore[i+1]= hscore[i];
    i++;
    hscore[i] = sc;
}
