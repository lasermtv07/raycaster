#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
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
    if(map[x+lineLen*y]==' ' || map[x+lineLen*y]=='P')
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
    TTF_Init();

    //import textures
    bst* texture=newNode('#',IMG_LoadTexture(rend,"gfx/brick.png"));
    addNode(texture,'O',IMG_LoadTexture(rend,"gfx/wood.png"));

const char* map="#################\n"\
"##      #      ##\n"\
"## # ## # # ## ##\n"\
"##             ##\n"\
"## # # ### # # ##\n"\
"##   #  #  #   ##\n"\
"#### ## # ## ####\n"\
"#### #     # ####\n"\
"#### # #O# # ####\n"\
" P     # #     P \n"\
"#### # ### # ####\n"\
"#### #     # ####\n"\
"#### # ### # ####\n"\
"##      #      ##\n"\
"## ## # # # ## ##\n"\
"##  #       #  ##\n"\
"### #  ###  # ###\n"\
"##      #      ##\n"\
"## #### # #### ##\n"\
"##             ##\n"\
"#################";

    //mik
    /*int* spriteW=malloc(sizeof(int));
    int* spriteH=malloc(sizeof(int));
    SDL_QueryTexture(miku,NULL,NULL,spriteW,spriteH);*/
    SDL_Texture *miku=IMG_LoadTexture(rend,"gfx/miku.png");
    SDL_Texture* teto=IMG_LoadTexture(rend,"gfx/teto.png");
    SDL_Texture* orb=IMG_LoadTexture(rend,"gfx/orb.png");
    SDL_Texture* blinky=IMG_LoadTexture(rend,"gfx/blinky.png");
    SDL_Texture* pinky=IMG_LoadTexture(rend,"gfx/pinky.png");
    SDL_Texture* inky=IMG_LoadTexture(rend,"gfx/inky.png");
    SDL_Texture* clyde=IMG_LoadTexture(rend,"gfx/clyde.png");
    SDL_Texture* pellet=IMG_LoadTexture(rend,"gfx/orb2.png");
    SDL_Texture* gun=IMG_LoadTexture(rend,"gfx/gun.png");
    SDL_Texture* frightened=IMG_LoadTexture(rend,"gfx/frightened.png");
    SDL_Texture* bullet=IMG_LoadTexture(rend,"gfx/bullets.png");
    SDL_Texture* heart=IMG_LoadTexture(rend,"gfx/hearth.png");
    TTF_Font* font=TTF_OpenFont("font.ttf",16);

    //MIXER DEEZ NUTS
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
    Mix_Chunk* collect=Mix_LoadWAV("sfx/pickupCoin.wav");
    Mix_Chunk* collectPellet=Mix_LoadWAV("sfx/powerUp.wav");
    Mix_Chunk* shoot=Mix_LoadWAV("sfx/laserShoot.wav");
    Mix_Chunk* hit=Mix_LoadWAV("sfx/hitHurt.wav");
    Mix_Chunk* playerHit=Mix_LoadWAV("sfx/hitPlayer.wav");

    sprite* spriteList=newSprite(blinky,8*32+8,9*32+8,0);
    addSprite(spriteList,pinky,8*32+8,9*32+8,0);
    addSprite(spriteList,inky,8*32+8,9*32+8,0);
    addSprite(spriteList,clyde,8*32+8,9*32+8,0);
    /*addSprite(spriteList,teto,169,169,0);
    addSprite(spriteList,miku,189,189,0);
    addSprite(spriteList,miku,1*32+12,38,0);
    addSprite(spriteList,miku,7*32,4*32+10,0);*/
    spriteSort(spriteList);

    int bullets=0;
    bool clydeRunAway=false;
    bool chase=true;
    bool switchChase=true;

    //place orbs
    int Xshift=0;
    int Yshift=0;
    for(int i=0;i<strlen(map);i++){
        if((Yshift==4 && Xshift==2) || (Yshift==4 && Xshift==14)
        || (Yshift==18 && Xshift==2) || (Yshift==18 && Xshift==14)
)
            addSprite(spriteList,pellet,Xshift*32,Yshift*32,0);
        else if(map[i]==' ' && i!=170 && i!=162 && i!=162+16)
            addSprite(spriteList,orb,Xshift*32,Yshift*32,0);
        Xshift++;
        if(map[i]=='\n'){
            Xshift=0;
            Yshift++;
        }
    }
    
    SDL_RenderClear(rend);

    //line length
    int lineLen=0;
    for(int i=0;i<strlen(map);i++){
        lineLen++;
        if(map[i]=='\n')
            break;
    }
    //init vars
    const int ps=3;
    int px=108;
    int py=48;
    float angle=0.05;
    int lives=3;
    int floorColor[]={160,160,160};
    int ceilingColor[]={0,0,255};
    int immunity=0;
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
        //printf("%d\n",doesPointIntersect(mX,mY,map));
        SDL_SetRenderDrawColor(rend,255,0,0,255);
        SDL_RenderFillRect(rend,&(SDL_Rect){x:px,y:py,w:24,h:24});
        SDL_RenderDrawLine(rend,px+12,py+12,px+12+32*cos(angle),py+12+32*sin(angle));
        //SDL_RenderDrawRect(rend,&(SDL_Rect){x:(px/8)*8+3,y:(py/8)*8+3,w:8,h:8});
        bool wasShot=false;
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                SDL_DestroyWindow(win);
                Mix_CloseAudio();
                SDL_Quit();
                return 0;
            }
            if(event.type==SDL_KEYDOWN){
                if(SDL_GetKeyFromScancode(event.key.keysym.scancode)==SDLK_SPACE && bullets>0){
                    wasShot=true;
                    bullets--;
                    Mix_PlayChannel(-1,shoot,0);
                }
            }
        }
        //printf("%c\n",mapAt(5,3,map));

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

    //printf("%f\n",angle);
    //calculate vector components and angles
    sprite* tmpSprite=spriteList;

    //calculate sprite info
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
        //printf("%f\n",q);
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
           //renders sprites
           sprite* tmpSprite2=spriteList;
           while(tmpSprite2!=NULL){
                if(tmpSprite2->visible){
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
                    for(int i=-sprW/2;i<sprW/2;i++){
                        int column=(x+i*4*(scanlineCount/sprW))/4;
                        if(column>=0 && column<320){
                            SDL_Texture* currentSprite=tmpSprite2->texture;
                            if(tmpSprite2->frightened)
                                currentSprite=frightened;
                            if(lengths[column]>dst){
                                SDL_RenderCopy(rend,currentSprite,&(SDL_Rect){w:1,h:sprH,x:i+sprW/2,y:0},&(SDL_Rect){w:4+4*(scanlineCount/sprW),h:h,y:y,x:x+i*4*(scanlineCount/sprW)});

                            }
                        }
                    }
                    //collision with orbs
                    if(tmpSprite2->texture==orb){
                        int gameX=tmpSprite2->x;
                        int gameY=tmpSprite2->y;
                        if(gameX>px-16 && gameX<px+16)
                            if(gameY>py-16 && gameY<py+16){
                                tmpSprite2->visible=false;
                                Mix_PlayChannel(-1,collect,0);
                            }
                    }
                    //collision with pellets
                    if(tmpSprite2->texture==pellet){
                        int gameX=tmpSprite2->x;
                        int gameY=tmpSprite2->y;
                        if(gameX>px-16 && gameX<px+16)
                            if(gameY>py-16 && gameY<py+16){
                                tmpSprite2->visible=false;
                                bullets++;
                                Mix_PlayChannel(-1,collectPellet,0);
                            }
                    }
                }
                tmpSprite2=tmpSprite2->next;
           }
           //gun collision
           sprite* tmpSprite5=spriteList;
            while(tmpSprite5->next!=NULL)
                tmpSprite5=tmpSprite5->next;
            while(tmpSprite5!=NULL){
                    float dst=tmpSprite5->dst;
                    float projX=tmpSprite5->posX;

                    float w=32*720/dst;
                    float h=64*720/dst;
                    float x=1280-projX;
                    float y=360-(64*720/dst)/2;

                    if(x>640-120 && x<640+w+120){
                        if(tmpSprite5->texture==inky || tmpSprite5->texture==blinky
                        || tmpSprite5->texture==pinky || tmpSprite5->texture==clyde){
                            if(wasShot){
                                tmpSprite5->frightened=true;
                                wasShot=false;
                                Mix_PlayChannel(-1,hit,0);
                            }
                        }
                    }
                tmpSprite5=tmpSprite5->prev;
            }

    int currX=floor((px)/32);
    int currY=floor((py+12)/32);

    SDL_SetRenderDrawColor(rend,0,0,0,255);
     //draw map
        int yshift=0;
        int xshift=0;
        for(int i=0;i<strlen(map);i++){
            if(map[i]!=' ' && map[i]!='\n')
                SDL_RenderFillRect(rend,&(SDL_Rect){x:xshift*8,y:yshift*8,w:8,h:8});
            else if(map[i]!='\n')
                SDL_RenderDrawRect(rend,&(SDL_Rect){x:xshift*8,y:yshift*8,w:8,h:8});
            xshift+=1;
            if(map[i]=='\n'){
                yshift+=1;
                xshift=0;
            }
    }
    SDL_SetRenderDrawColor(rend,255,187,0,255);
    SDL_RenderFillRect(rend,&(SDL_Rect){x:(px/4)+3,y:(py/4)+3,w:8,h:8});

    //draw sprites, handle movements
    int remaining=0;
    SDL_SetRenderDrawColor(rend,255,251,0,255);
    sprite* tmpSprite3=spriteList;
    while(tmpSprite3!=NULL){
        if(tmpSprite3->visible && tmpSprite3->texture==orb){
            int lX=tmpSprite3->x;
            int lY=tmpSprite3->y;
            SDL_RenderFillRect(rend,&(SDL_Rect){x:lX/4+2,y:lY/4+2,w:4,h:4});
            remaining++;
        }
        if(tmpSprite3->visible && tmpSprite3->texture==pellet){
            int lX=tmpSprite3->x;
            int lY=tmpSprite3->y;
            SDL_SetRenderDrawColor(rend,255,128,128,255);
            SDL_RenderFillRect(rend,&(SDL_Rect){x:lX/4+2,y:lY/4+2,w:4,h:4});
            SDL_SetRenderDrawColor(rend,255,251,0,255);
            remaining++;
        }
        if(tmpSprite3->texture==blinky || tmpSprite3->texture==pinky
           || tmpSprite3->texture==inky || tmpSprite3->texture==clyde){

            if(tmpSprite3->x==8*32+8 && tmpSprite3->y==9*32+8)
                tmpSprite3->frightened=false;

            if(tmpSprite3->x>tmpSprite3->trgX)
                tmpSprite3->x--;
            if(tmpSprite3->x<tmpSprite3->trgX)
                tmpSprite3->x++;
            if(tmpSprite3->y>tmpSprite3->trgY)
                tmpSprite3->y--;
            if(tmpSprite3->y<tmpSprite3->trgY)
                tmpSprite3->y++;

            //dir=0: up
            //dir=1: left
            //dir=2: right
            //dir=3: down
            if(tmpSprite3->y==tmpSprite3->trgY && tmpSprite3->x==tmpSprite3->trgX){
                int ghostX=tmpSprite3->x;
                int ghostY=tmpSprite3->y;
                bool possU=true;
                bool possL=true;
                bool possR=true;
                bool possD=true;
                int dir=tmpSprite3->dir;

                if(dir==0)
                    possD=false;
                if(dir==1)
                    possR=false;
                if(dir==2)
                    possL=false;
                if(dir==3)
                    possU=false;
                
                int gCellX=floor(ghostX/32);
                int gCellY=floor(ghostY/32);

                if(mapAt(gCellX+1,gCellY,map)!=' ')
                    possR=false;
                if(mapAt(gCellX-1,gCellY,map)!=' ')
                    possL=false;
                if(mapAt(gCellX,gCellY+1,map)!=' ' && mapAt(gCellX,gCellY+1,map)!='O')
                    possD=false;
                if(mapAt(gCellX,gCellY-1,map)!=' ' && !(mapAt(gCellX,gCellY-1,map)=='O' && tmpSprite3->frightened))
                    possU=false;

                if(!(possD||possU||possL||possR)){
                if(dir==0)
                    possD=true;
                if(dir==1)
                    possR=true;
                if(dir==2)
                    possL=true;
                if(dir==3)
                    possU=true;
                }

                int tx;
                int ty;
                //chase mode
                if(chase){
                    if(tmpSprite3->texture==blinky){
                        tx=px;
                        ty=py;
                    }
                    if(tmpSprite3->texture==pinky){
                        if(angle>45*M_PI/180 && angle<135*M_PI/180){
                            tx=px;
                            ty=4*32+py;
                        }
                        if((angle>225*M_PI/180 && angle<315*M_PI/180)){
                            tx=px;
                            ty=py-4*32;
                        }
                        if((angle>135*M_PI/180 && angle<225*M_PI/180)){
                            tx=px-4*32;
                            ty=py;
                        }
                        if((angle>0 && angle<45*M_PI/180) || (angle>315*M_PI/180 && angle<360*M_PI/180)){
                            tx=px+4*32;
                            ty=py;
                        }                   
                    }
                    //TODO: check correct
                    if(tmpSprite3->texture==inky){
                        //git blinky x
                        int blinkyX;
                        int blinkyY;
                        sprite* tmpSprite4=spriteList;
                        while(tmpSprite4!=NULL){
                            blinkyX=tmpSprite4->x;
                            blinkyY=tmpSprite4->y;
                            tmpSprite4=tmpSprite4->next;
                        }
                        //D.R.Y DEEZ NUTS
                        int ppx;
                        int ppy;
                        if(angle>45*M_PI/180 && angle<135*M_PI/180){
                            ppx=px;
                            ppy=2*32+py;
                        }
                        if((angle>225*M_PI/180 && angle<315*M_PI/180)){
                            ppx=px;
                            ppy=py-2*32;
                        }
                        if((angle>135*M_PI/180 && angle<225*M_PI/180)){
                            ppx=px-2*32;
                            ppy=py;
                        }
                        if((angle>0 && angle<45*M_PI/180) || (angle>315*M_PI/180 && angle<360*M_PI/180)){
                            ppx=px+2*32;
                            ppy=py;
                        }    

                        float dx=-blinkyX+ppx;
                        float dy=-blinkyY+ppy;

                        tx=ppx+dx;
                        ty=ppy+dy;
                    }
                    if(tmpSprite3->texture==clyde){
                        if(!clydeRunAway){
                            tx=px;
                            ty=py;
                        }
                        else {
                            tx=-10000;
                            ty=10000;
                        }
                        int dx=tmpSprite3->x-px;
                        int dy=tmpSprite3->y-py;
                        if(sqrt(dx*dx+dy*dy)<4*32){
                            clydeRunAway=true;
                        }
                        if(gCellX==2 && gCellY==19){
                            clydeRunAway=false;
                        }

                    }
                }
                //scatter mode
                else {
                    if(tmpSprite3->texture==pinky){
                        tx=-1000;
                        ty=-1000;
                    }
                    if(tmpSprite3->texture==blinky){
                        tx=1000;
                        ty=-1000;
                    }
                    if(tmpSprite3->texture==clyde){
                        tx=-1000;
                        ty=1000;
                    }
                    if(tmpSprite3->texture==inky){
                        tx=1000;
                        ty=1000;
                    }
                }
                if(tmpSprite3->frightened){
                    tx=8*32+8;
                    ty=9*32+8;
                }
                float dstL=99999999;
                float dstR=99999999;
                float dstU=99999999;
                float dstD=99999999;

                if(possL)
                    dstL=sqrt(((gCellX-1)*32-tx)*((gCellX-1)*32-tx)+(gCellY*32-ty)*(gCellY*32-ty));
                if(possR)
                    dstR=sqrt(((gCellX+1)*32-tx)*((gCellX+1)*32-tx)+(gCellY*32-ty)*(gCellY*32-ty));
                if(possU)
                    dstU=sqrt(((gCellX)*32-tx)*((gCellX)*32-tx)+((gCellY-1)*32-ty)*((gCellY-1)*32-ty));
                if(possD)
                    dstD=sqrt(((gCellX)*32-tx)*((gCellX)*32-tx)+((gCellY+1)*32-ty)*((gCellY+1)*32-ty));

                float min=dstL;
                if(dstR<min)
                    min=dstR;
                if(dstU<min)
                    min=dstU;
                if(dstD<min)
                    min=dstD;

                if(min==dstU){
                    tmpSprite3->trgX=(gCellX)*32+8;
                    tmpSprite3->trgY=(gCellY-1)*32+8;
                    tmpSprite3->dir=0;
                    //printf("Go UP!\n");
                }
                if(min==dstL){
                    tmpSprite3->trgX=(gCellX-1)*32+8;
                    tmpSprite3->trgY=gCellY*32;
                    tmpSprite3->dir=1;
                    //printf("Go LEFT!\n");
                }
                if(min==dstD){
                    tmpSprite3->trgX=(gCellX)*32+8;
                    tmpSprite3->trgY=(gCellY+1)*32+8;
                    tmpSprite3->dir=3;
                    //printf("Go DOWN!\n");
                }
                if(min==dstR){
                    tmpSprite3->trgX=(gCellX+1)*32+8;
                    tmpSprite3->trgY=gCellY*32;
                    tmpSprite3->dir=2;
                    //printf("Go RIGHT!\n");
                }
                
                SDL_SetRenderDrawColor(rend,255,0,0,255);
                SDL_RenderFillRect(rend,&(SDL_Rect){w:8,h:8,x:tmpSprite3->x/4,y:tmpSprite3->y/4});
            }
            if(sqrt((px-tmpSprite3->x)*(px-tmpSprite3->x)+(py-tmpSprite3->y)*(py-tmpSprite3->y))<16){
                if(!tmpSprite3->frightened){
                    tmpSprite3->frightened=true;
                    lives--;
                    Mix_PlayChannel(-1,playerHit,0);
                    immunity=60;
                }
            }
        }
/*        if(tmpSprite3->texture==blinky || tmpSprite3->texture==pinky
           || tmpSprite3->texture==inky || tmpSprite3->texture==clyde){
                SDL_SetRenderDrawColor(rend,255,0,0,255);
                SDL_RenderFillRect(rend,&(SDL_Rect){w:8,h:8,x:tmpSprite3->x/4,y:tmpSprite3->y/4});
           }*/
                tmpSprite3=tmpSprite3->next;
        SDL_SetRenderDrawColor(rend,255,251,0,255);
    }
    SDL_SetRenderDrawColor(rend,255,255,255,255);
    //UI orb remaining
    char* orbStr=malloc(1204);
    memset(orbStr,0,1024);
    sprintf(orbStr,"%d",remaining);
    SDL_Surface* orbSurf=TTF_RenderText_Solid(font,orbStr,(SDL_Color){r:255,g:255,b:255,a:255});
    SDL_Texture* orbTex=SDL_CreateTextureFromSurface(rend,orbSurf);
    SDL_RenderCopy(rend,orbTex,NULL,&(SDL_Rect){w:(100/3)*strlen(orbStr),h:50,x:30*8,y:32});
    SDL_RenderCopy(rend,orb,NULL,&(SDL_Rect){w:64*2,h:120*2,x:17*8,y:-80});
    SDL_DestroyTexture(orbTex);
    SDL_FreeSurface(orbSurf);
    free(orbStr);

    //bullets UI
    char* bulStr=malloc(1024);
    memset(bulStr,0,1024);
    sprintf(bulStr,"%d",bullets);
    SDL_Surface* bulSurf=TTF_RenderText_Solid(font,bulStr,(SDL_Color){r:255,g:255,b:255,a:255});
    SDL_Texture* bulTex=SDL_CreateTextureFromSurface(rend,bulSurf);
    SDL_RenderCopy(rend,bulTex,NULL,&(SDL_Rect){w:(100/3)*strlen(bulStr),h:50,x:70*8,y:32});
    SDL_RenderCopy(rend,bullet,NULL,&(SDL_Rect){w:32*2,h:32*2,x:60*8,y:20});
    SDL_DestroyTexture(bulTex);
    SDL_FreeSurface(bulSurf);
    free(bulStr);
   
    //lives UI
    SDL_RenderCopy(rend,heart,NULL,&(SDL_Rect){w:32*2,h:32*2,x:100*8,y:20});
    char* livStr=malloc(1024);
    memset(livStr,0,1024);
    sprintf(livStr,"%d",lives);
    SDL_Surface* livSurf=TTF_RenderText_Solid(font,livStr,(SDL_Color){r:255,g:255,b:255,a:255});
    SDL_Texture* livTex=SDL_CreateTextureFromSurface(rend,livSurf);
    SDL_RenderCopy(rend,livTex,NULL,&(SDL_Rect){w:(100/3)*strlen(livStr),h:50,x:110*8,y:32});

    if(bullets>0)
        SDL_RenderCopy(rend,gun,NULL,NULL);

    SDL_SetRenderDrawColor(rend,0,0,0,255);
    SDL_RenderPresent(rend);
    SDL_Delay((1000/60));
    if(angle>2*M_PI)
        angle-=2*M_PI;
    if(angle<0)
        angle+=2*M_PI;

    //switches mode
    if((int)time(NULL)%7==0 && switchChase){
        chase=!chase;
        switchChase=false;
    }
    else if((int)time(NULL)%7!=0)
        switchChase=true;

    //handles DOOORSS!
    if(currX==1 && currY==9){
        px=14*32+16-32;
        py=8*32+48;
        angle=180*(M_PI/180);
    }
    if(currX==14 && currY==9){
        px=64+16+32;
        py=8*32+48;
        angle=0*(M_PI/180);
    }
    
    if(immunity>0)
        immunity--;
    }
    return 0;
}