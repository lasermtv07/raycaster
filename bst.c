#pragma once
#include <stdlib.h>
#include <SDL2/SDL.h>
typedef struct bst {
    char symbol;
    SDL_Texture* tex;
    struct bst* left;
    struct bst* right;
} bst;

bst* newNode(char symbol,SDL_Texture* tex){
    bst* o=malloc(sizeof(bst));
    if(o!=NULL){
        o->symbol=symbol;
        o->tex=tex;
        o->left=NULL;
        o->right=NULL;
    }
    return o;
}

void addNode(bst* a,char symbol,SDL_Texture* tex){
    if(symbol<a->symbol){
        if(a->left==NULL)
            a->left=newNode(symbol,tex);
        else
            addNode(a->left,symbol,tex);
    }
    else {
        if(a->right==NULL)
            a->right=newNode(symbol,tex);
        else
            addNode(a->right,symbol,tex);
    }
    return;
}

SDL_Texture* searchNode(bst* a,char symbol){
    if(a==NULL)
        return NULL;
    if(a->symbol==symbol)
        return a->tex;
    else if(symbol<a->symbol)
        return searchNode(a->left,symbol);
    else
        return searchNode(a->right,symbol);
}