#include <stdlib.h>
#include <SDL2/SDL.h>

typedef struct sprite {
    SDL_Texture* texture;
    int x;
    int y;
    int z;
    float dst;
    float posX;
    float angle;
    bool visible;
    int trgX;
    int trgY;
    int dir;
    bool frightened;
    struct sprite* next;
    struct sprite* prev;
} sprite;

sprite* newSprite(SDL_Texture* texture,int x, int y, int z){
    sprite* o=malloc(sizeof(sprite));
    if(o!=NULL){
        o->texture=texture;
        o->x=x;
        o->y=y;
        o->z=z;
        o->dst=0;
        o->posX=0;
        o->angle=0;
        o->visible=true;
        o->trgX=x;
        o->trgY=y;
        o->dir=0;
        o->frightened=false;
        o->next=NULL;
        o->prev=NULL;
    }
    return o;
}

void addSprite(sprite* a,SDL_Texture* texture,int x, int y, int z){
    while(a->next!=NULL)
        a=a->next;
    a->next=newSprite(texture,x,y,z);
    (a->next)->prev=a;
}
void copySpriteData(sprite* s2, sprite* s1){
        s2->texture=s1->texture;
        s2->x=s1->x;
        s2->y=s1->y;
        s2->z=s1->z;
        s2->dst=s1->dst;
        s2->posX=s1->posX;
        s2->visible=s1->visible;
        s2->trgX=s1->trgX;
        s2->trgY=s1->trgY;
        s2->dir=s1->dir;
        s2->frightened=s1->frightened;
}
void spriteSort(sprite* a){
    if(a==NULL)
        return;
    sprite* tmp1=a;
    while(tmp1!=NULL){
        sprite* tmp2=tmp1;
        sprite* max=tmp2;
        while(tmp2!=NULL){
            if(max->dst < tmp2->dst)
                max=tmp2;
            tmp2=tmp2->next;
        }

        sprite* temp=malloc(sizeof(sprite));
        copySpriteData(temp,tmp1);
        copySpriteData(tmp1,max);
        copySpriteData(max,temp);
        free(temp);

        tmp1=tmp1->next;
    }
}