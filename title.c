#include"title.h"

#define INTERNAL 

static SDL_Window * window = 0;
static SDL_Thread *thread = 0;
static int quit = 0;
int INTERNAL changeTitle(void * d)
{
    //当程序还未结束
    while( !quit)
    {
        //运行标题动画
        SDL_SetWindowTitle( window,"run." );
        SDL_Delay( 250 );

        SDL_SetWindowTitle( window,"run..");
        SDL_Delay( 250 );

        SDL_SetWindowTitle(window, "run...");
        SDL_Delay( 250 );

        SDL_SetWindowTitle( window,"run....");
        SDL_Delay( 250 );
    }

    return 0;
}

int init(SDL_Window * w) {
	quit = 0;
	window = w;
}

int startChangeTitle() {
	if(window != 0) {
		 thread = SDL_CreateThread( changeTitle, "changetitleThread",NULL );
	}else {
		printf("title.c:there isn't a window so can't start");
	}
}


int stopChangeTitle() {
	quit =1;
}


