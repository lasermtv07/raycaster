#include <stdlib.h>
#include <SDL2/SDL.h>

typedef struct sprite {
    SDL_Texture* texture;
    int x;
    int y;
    int z;
    float dst;
    float posX;
    struct sprite* next;
} sprite;

sprite* newSprite(SDL_Texture* texture,int x, int y, int z){
    sprite* o=malloc(sizeof(sprite));
    if(o!=NULL){
        o->x=x;
        o->y=y;
        o->z=z;
        o->dst=0;
        o->posX=0;
        o->next=NULL;
    }
    return o;
}

void addSprite(sprite* a,SDL_Texture* texture,int x, int y, int z){
    while(a->next!=NULL)
        a=a->next;
    a->next=newSprite(texture,x,y,z);
}
void spriteSort();
