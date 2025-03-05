#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define M_PI 3.14159265358979323846

float min(float a,float b){
    if(a>b)
        return b;
    else
        return a;
}
bool doesPointIntersect(int x,int y,const char* map){
    int lineLen=0;
    for(int i=0;i<strlen(map);i++){
        lineLen++;
        if(map[i]=='\n')
            break;
    }
    x=x/32;
    y=y/32;
    if(x>lineLen*32 || y>strlen(map)/lineLen)
        return false;
    if(map[x+lineLen*y]==' ')
        return false;
    else
        return true;
}
char mapAt(int x,int y,const char* map){
     int lineLen=0;
    for(int i=0;i<strlen(map);i++){
        lineLen++;
        if(map[i]=='\n')
            break;
    }
    return map[lineLen*y+x];
}
int main(){
    
    if(SDL_Init(SDL_INIT_VIDEO| SDL_INIT_TIMER | SDL_INIT_AUDIO)){
        printf("error\n");
        return 1;
    }
    SDL_Window* win=SDL_CreateWindow("game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,0);
    if(!win){
        printf("window error\n");
        return 1;
    }
    SDL_Renderer* rend=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderClear(rend);
    if(!rend){
        printf("error creating renderer\n");
        return 1;
    }


    SDL_RenderClear(rend);
    const char* map="##########\n"\
               "#        #\n"\
               "#     ## #\n"\
               "#    #   #\n"\
               "#        #\n"\
               "#        #\n"\
               "##########";
    int lineLen=0;
    for(int i=0;i<strlen(map);i++){
        lineLen++;
        if(map[i]=='\n')
            break;
    }
    const int ps=3;
    int px=72;
    int py=48;
    float angle=0;
    while(true){
        //int elapsed=SDL_GetTicks();
        SDL_RenderClear(rend);

        int currentX=(int)((px+2*cos(angle))/32);
        int currentY=(int)((py+2*sin(angle))/32);

        int yshift=0;
        int xshift=0;
        SDL_SetRenderDrawColor(rend,0,0,255,255);
        for(int i=0;i<strlen(map);i++){
            if(map[i]!=' ' && map[i]!='\n')
                SDL_RenderFillRect(rend,&(SDL_Rect){x:xshift*32,y:yshift*32,w:32,h:32});
            else
                SDL_RenderDrawRect(rend,&(SDL_Rect){x:xshift*32,y:yshift*32,w:32,h:32});
            xshift+=1;
            if(map[i]=='\n'){
                yshift+=1;
                xshift=0;
            }
        }

        int mX;
        int mY;
        SDL_GetMouseState(&mX,&mY);
        printf("%d\n",doesPointIntersect(mX,mY,map));
        SDL_SetRenderDrawColor(rend,255,0,0,255);
        SDL_RenderFillRect(rend,&(SDL_Rect){x:px,y:py,w:24,h:24});
        SDL_RenderDrawLine(rend,px+12,py+12,px+12+32*cos(angle),py+12+32*sin(angle));
        //SDL_RenderDrawRect(rend,&(SDL_Rect){x:(px/32)*32+12,y:(py/32)*32+12,w:32,h:32});
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                SDL_DestroyWindow(win);
                Mix_CloseAudio();
                SDL_Quit();
                return 0;
            }
        }
        printf("%c\n",mapAt(5,3,map));
    //handle key controls
    
    const Uint8* kb=SDL_GetKeyboardState(NULL);
    if(kb[SDL_SCANCODE_A])
        angle-=0.05;
    if(kb[SDL_SCANCODE_D])
        angle+=0.05;
SDL_SetRenderDrawColor(rend,0,255,0,255);
    if(kb[SDL_SCANCODE_W]){
        //todo:L finish
        bool left=doesPointIntersect(px,py+2,map) || doesPointIntersect(px,py+22,map);
        bool right=doesPointIntersect(px+24,py+2,map) || doesPointIntersect(px+24,py+22,map);
        bool top=doesPointIntersect(px+2,py,map) || doesPointIntersect(px+22,py,map);
        bool bottom=doesPointIntersect(px+2,py+24,map) || doesPointIntersect(px+22,py+24,map);
        if(doesPointIntersect(px+1,py+5,map))
            px+=1;
        if(doesPointIntersect(px+23,py+5,map))
            px-=1;
        if(doesPointIntersect(px+5,py+1,map))
            py+=1;
        if(doesPointIntersect(px+5,py+23,map))
            py-=1;
        float dX=ps*cos(angle);
        float dY=ps*sin(angle);
        if((dX<0 && !left) || (dX>0 && !right))
            px+=ps*cos(angle);
        if((dY<0 && !top) || (dY>0 && !bottom))
        py+=ps*sin(angle);
    }

    //lines
    
    SDL_SetRenderDrawColor(rend,0,255,0,255);
    float dirX=cos(angle);
    float dirY=sin(angle);

    int shiftX;
    int shiftY;
    if(dirX<0)
        shiftX=-1;
    else
        shiftX=1;
    if(dirY<0)
        shiftY=-1;
    else
        shiftY=1;

    printf("%f\n",cos(angle));
    int currX=floor((px+12)/32);
    int currY=floor((py+12)/32);
    SDL_SetRenderDrawColor(rend,255,0,0,255);
    SDL_RenderDrawRect(rend,&(SDL_Rect){x:32*currX,y:32*currY,w:32,h:32});
    SDL_SetRenderDrawColor(rend,0,255,0,255);
    float initX=1-((((float) px+12)-32.0*((float)floor((double)((px+12)/32))))/32);
    float initY=1-((((float) py+12)-32.0*((float)floor((double)((py+12)/32))))/32);
    int side;

    if(shiftX<0){
        initX=1-initX;
        side=0;
    }
    if(shiftY<0){
        initY=1-initY;
        side=1;
    }

    float stepY=initY*sqrt((1/dirY)*(1/dirY));
    float stepX=initX*sqrt((1/dirX)*(1/dirX));
    float unitStepX=sqrt((1/dirX)*(1/dirX));
    float unitStepY=sqrt((1/dirY)*(1/dirY));

    printf("%f -> %f\n",(1+initX)*stepX,(1+initY)*stepY);

    //SDL_SetRenderDrawColor(rend,120,0,120,255);
    if(stepX<stepY){
        currX+=shiftX;
        stepX+=unitStepX;
        side=0;
    }
    else {
        currY+=shiftY;
        stepY+=unitStepY;
        side=1;
    }
        bool cont=true;
        if(mapAt(currX,currY,map)!=' '){
            //SDL_RenderDrawRect(rend,&(SDL_Rect){x:currX*32,y:currY*32,w:32,h:32});
            if(side==0)
                SDL_RenderDrawLine(rend,px+12,py+12,px+12+(stepX-unitStepX)*dirX*32,py+12+(stepX-unitStepX)*dirY*32);
            else
                SDL_RenderDrawLine(rend,px+12,py+12,px+12+(stepY-unitStepY)*dirX*32,py+12+(stepY-unitStepY)*dirY*32);
            cont=false;
        }
    //SDL_RenderDrawRect(rend,&(SDL_Rect){x:currX*32,y:currY*32,w:32,h:32});


    for(int i=1;i<20 && cont;i++){
        float cacheX=stepX;
        float cacheY=stepY;
        if(stepX<stepY){
            currX+=shiftX;
            stepX+=unitStepX;
            side=0;
        }
        else {
            currY+=shiftY;
            stepY+=unitStepY;
            side=1;
        }
        if(mapAt(currX,currY,map)!=' '){
            SDL_RenderDrawRect(rend,&(SDL_Rect){x:currX*32,y:currY*32,w:32,h:32});
            if(side==0)
                SDL_RenderDrawLine(rend,px+12,py+12,px+12+cacheX*dirX*32,py+12+cacheX*dirY*32);
            else
                SDL_RenderDrawLine(rend,px+12,py+12,px+12+cacheY*dirX*32,py+12+cacheY*dirY*32);
            break;
        }
    }
    //SDL_RenderDrawLine(rend,px+12,py+12,px+12+dirX*20*32,py+12+dirY*20*32);


    SDL_SetRenderDrawColor(rend,0,0,0,255);
    SDL_RenderPresent(rend);
    SDL_Delay((1000/60));
    }
    return 0;
}