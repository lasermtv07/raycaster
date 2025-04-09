#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sprites.c"
#include "bst.c"
#define M_PI 3.14159265358979323846

int quadrant(float angle){
    if(angle<0)
        angle+=2*M_PI;

    if(angle>0 && angle<0.5*M_PI)
        return 1;
    if(angle>0.5*M_PI && angle<M_PI)
        return 2;
    if(angle>M_PI && angle<1.5*M_PI)
        return 3;
    if(angle>1.5*M_PI && angle<2*M_PI)
        return 4;

    return -1;
}
float min(float a,float b){
    if(a>b)
        return b;
    else
        return a;
}
float fAbs(float a){
    if(a>=0)
        return a;
    else
        return -a;
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
int main(){/*
    //init
    if(SDL_Init(SDL_INIT_VIDEO| SDL_INIT_TIMER | SDL_INIT_AUDIO)){
        printf("error\n");
        return 1;
    }*/
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
    SDL_SetRenderDrawBlendMode(rend,SDL_BLENDMODE_BLEND);

    //import textures
    bst* texture=newNode('#',IMG_LoadTexture(rend,"gfx/brick.png"));
    addNode(texture,'O',IMG_LoadTexture(rend,"gfx/wood.png"));

    //mik
    /*int* spriteW=malloc(sizeof(int));
    int* spriteH=malloc(sizeof(int));
    SDL_QueryTexture(miku,NULL,NULL,spriteW,spriteH);*/
    SDL_Texture *miku=IMG_LoadTexture(rend,"gfx/miku.png");
    SDL_Texture* teto=IMG_LoadTexture(rend,"gfx/teto.png");

    sprite* spriteList=newSprite(miku,64,64,0);
    addSprite(spriteList,teto,169,169,0);
addSprite(spriteList,miku,189,189,0);
addSprite(spriteList,miku,7*32+12,38,0);
addSprite(spriteList,miku,7*32,4*32+10,0);
    spriteSort(spriteList);



    SDL_RenderClear(rend);
    const char* map="##########\n"\
               "#        #\n"\
               "#    OOO #\n"\
               "#  OOO  ##\n"\
               "#  O  O ##\n"\
               "#  O    ##\n"\
               "#  OO    #\n"\
               "#   OO   #\n"\
               "#        #\n"\
               "##########";
    
    //line length
    int lineLen=0;
    for(int i=0;i<strlen(map);i++){
        lineLen++;
        if(map[i]=='\n')
            break;
    }
    //init vars
    const int ps=3;
    int px=72;
    int py=48;
    float angle=0.05;
    int floorColor[]={160,160,160};
    int ceilingColor[]={0,0,255};
    //int mikuX=6*32+25;
    //int mikuY=112;

    while(true){

        //int elapsed=SDL_GetTicks();
        SDL_RenderClear(rend);
        /*for(int i=0;i<32;i++){
            SDL_RenderCopy(rend,brick,&(SDL_Rect){w:1,h:32,x:i,y:0},&(SDL_Rect){w:12,h:32*12,x:i*12,y:0});
        }*/
        int currentX=(int)((px+2*cos(angle))/32);
        int currentY=(int)((py+2*sin(angle))/32);

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

    printf("%f\n",angle);

    //calculate vector components and angles
    sprite* tmpSprite=spriteList;

    while(tmpSprite!=NULL){
        int hx=tmpSprite->x-px;
        int hy=tmpSprite->y-py;
        float hAngle=atan2(hy,hx);
        if(hAngle<0)
            hAngle+=2*3.1415926;
        if(hAngle>2*3.1415926)
            hAngle-=2*3.1415926;
        SDL_SetRenderDrawColor(rend,128,0,128,255);
        SDL_RenderDrawLine(rend,px+12,py+12,px+12+60*cos(hAngle),py+12+60*sin(hAngle));

        //calculate angle from the rightmost 
        float q=angle+(1.047198/2)-hAngle;
        if(quadrant(angle)==1 && quadrant(hAngle)==4)
            q+=2*M_PI;
        if(quadrant(angle)==4 && quadrant(hAngle)==1)
            q-=2*M_PI;
        printf("%f\n",q);
        float projX=(1280/1.047198)*q;
        float dst=sqrt(hx*hx+hy*hy);

        tmpSprite->dst=dst;
        tmpSprite->posX=projX;
        tmpSprite->angle=q;
        tmpSprite=tmpSprite->next;
    }

    spriteSort(spriteList);
    //SDL_RenderCopy(rend,miku,NULL,&(SDL_Rect){w:32*720/dst,h:64*720/dst,y:360-(64*720/dst)/2,x:1280-projX});
    bool first=true;
    int lengths[320];
    for(int temp=2;temp>0;temp--){

        //RAYCASTER
        int column=0;
        int draw=false;
        for(float j=-0.5235988;j<0.5235988;j+=0.00327249375){

            //setup
            SDL_SetRenderDrawColor(rend,0,255,0,255);
            float dirX=cos(angle+j);
            float dirY=sin(angle+j);

            //coefficients to allow extension to 2pi rotation
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

            //calculating initial values
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

            //steps
            float stepY=initY*sqrt((1/dirY)*(1/dirY));
            float stepX=initX*sqrt((1/dirX)*(1/dirX));
            float unitStepX=fAbs(1/dirX);//sqrt((1/dirX)*(1/dirX));
            float unitStepY=fAbs(1/dirY);//sqrt((1/dirY)*(1/dirY));

            //initial step
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

            //special case: player is in a cell next to wall
            bool cont=true;
            float len;
            if(mapAt(currX,currY,map)!=' '){
                if(side==0){
                    len=(stepX-unitStepX);
                }
                else
                    len=(stepY-unitStepY);
                cont=false;
            }

            //extended raycast
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
                    //SDL_RenderDrawRect(rend,&(SDL_Rect){x:currX*32,y:currY*32,w:32,h:32});
                    if(side==0)
                        len=cacheX;
                    else
                        len=cacheY;
                    break;
                }
            }

            //calculate at which part of the cell the ray hit; for texturing
            int hitPosX=(px+12+dirX*len*32)-floor((px+12+dirX*len*32)/32)*32;
            int hitPosY=(py+12+dirY*len*32)-floor((py+12+dirY*len*32)/32)*32;

            //to mitigate fisheye effect
            float pa=j;
            if(pa<0)
                pa+=2*3.1415926;
            float scanColumn=(32*720)/((len*cos(pa))*32);
            float scanOffset=(16*720)/((len*cos(pa))*16);

            //SDL_RenderDrawLine(rend,px+12,py+12,px+12+dirX*len*32,py+12+dirY*len*32);

            if(first){
                //draw ceiling
                SDL_SetRenderDrawColor(rend,ceilingColor[0],ceilingColor[1],ceilingColor[2],255);
                SDL_RenderFillRect(rend,&(SDL_Rect){w:4,h:360-scanColumn/2,x:column*4,y:0});

                //draw floor
                SDL_SetRenderDrawColor(rend,floorColor[0],floorColor[1],floorColor[2],255);
                SDL_RenderFillRect(rend,&(SDL_Rect){w:4,h:720-(scanColumn+(360-scanColumn/2)),x:column*4,y:scanColumn+(360-scanColumn/2)});
            }

            if(/*(first && dst<len*32) || (!first && dst>len*32)*/first){

                //draw floor - redraw because reasons
                SDL_SetRenderDrawColor(rend,floorColor[0],floorColor[1],floorColor[2],255);
                SDL_RenderFillRect(rend,&(SDL_Rect){w:4,h:720-(scanColumn+(360-scanColumn/2)),x:column*4,y:scanColumn+(360-scanColumn/2)});

                //shading & drawing columns
                if(side==0){
                    SDL_SetRenderDrawColor(rend,0,0,0,1);
                }
                else {
                    SDL_SetRenderDrawColor(rend,0,0,0,128);
                }
                if(side==0)
                    SDL_RenderCopy(rend,searchNode(texture,mapAt(currX,currY,map)),&(SDL_Rect){w:1,h:32,x:hitPosY,y:0},&(SDL_Rect){w:4,h:scanColumn,x:column*4,y:360-scanColumn/2});
                else
                    SDL_RenderCopy(rend,searchNode(texture,mapAt(currX,currY,map)),&(SDL_Rect){w:1,h:32,x:hitPosX,y:0},&(SDL_Rect){w:4,h:scanColumn,x:column*4,y:360-scanColumn/2});
                if(side==1)
                    SDL_RenderFillRect(rend,&(SDL_Rect){w:4,h:scanColumn,x:column*4,y:360-scanColumn/2});
            lengths[column]=len*32;
            column++;
        }

    }
        if(first){
           //float dst=sqrt(hx*hx+hy*hy);
           sprite* tmpSprite2=spriteList;
           while(tmpSprite2!=NULL){
                float dst=tmpSprite2->dst;
                float projX=tmpSprite2->posX;

                float w=32*720/dst;
                float h=64*720/dst;
                float x=1280-projX;
                float y=360-(64*720/dst)/2;

                int sprW;
                int sprH;
                SDL_QueryTexture(tmpSprite2->texture,NULL,NULL,&sprW,&sprH);
                float scanlineCount=w/4;
                for(int i=0;i<sprW;i++){
                    int column=(x+i*4*(scanlineCount/sprW))/4;
                    if(column>=0 && column<320){
                        if(lengths[column]>dst)
                        SDL_RenderCopy(rend,tmpSprite2->texture,&(SDL_Rect){w:1,h:sprH,x:i,y:0},&(SDL_Rect){w:4+4*(scanlineCount/sprW),h:h,y:y,x:x+i*4*(scanlineCount/sprW)});
                    }
                }

                tmpSprite2=tmpSprite2->next;
           }
        }
        first=false;
    }
 /*   SDL_SetRenderDrawColor(rend,255,0,0,255);
    SDL_RenderFillRect(rend,&(SDL_Rect){w:10,h:10,x:mikuX,y:mikuY});
    SDL_SetRenderDrawColor(rend,0,0,0,255);
     //draw map
        int yshift=0;
        int xshift=0;
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
        }*/



    SDL_SetRenderDrawColor(rend,0,0,0,255);
    SDL_RenderPresent(rend);
    SDL_Delay((1000/60));
    if(angle>2*M_PI)
        angle-=2*M_PI;
    if(angle<0)
        angle+=2*M_PI;
    }
    return 0;
}