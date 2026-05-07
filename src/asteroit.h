#ifndef ASTEROIT_H
#define ASTEROIT_H
#include <SDL2/SDL.h>
#include "gemi.h"

#define ASTEROIT_SAYISI 7
#define ASTEROIT_BOY 40
#define ASTEROIT_MAX_HIZ 4
#define ASTEROIT_MIN_HIZ 1

typedef struct {
    float x; 
    float y; 
    SDL_Rect sekil; 
    float hizX;
    float hizY;
    int kontrol;

}Asteroit;

void asteroit_baslangic(Asteroit *asteroid);
void asteroit_uret(Asteroit *asteroit);
void asteroit_guncelle(Asteroit *asteroit);
void asteroit_ciz(SDL_Renderer *renderer, Asteroit *asteroit);
int asteroit_carpisma_kontrol(Asteroit *asteroit, struct Gemi *gemiPtr);






#endif
