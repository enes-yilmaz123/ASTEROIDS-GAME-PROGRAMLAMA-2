#ifndef SUPPLIES_H
#define SUPPLIES_H

#include <SDL2/SDL.h> 
#include "gemi.h"      

#define SUPPLY_MAX_SAYISI 3
#define SUPPLY_BOYUT 60

typedef enum
{
    SUPPLY_REPAIR = 1,
    SUPPLY_SHIELD = 2
} SupplyTuru;

typedef struct
{
    SDL_Rect sekil;
    int kontrol;
    SupplyTuru tur;
} Supply;

void supply_baslangic(Supply *supply);
void supply_uret(Supply *supply);
void supply_ciz(SDL_Renderer *renderer , Supply *supply);
void supply_toplama_kontrol(Supply *supply, struct Gemi *gemiPtr, int *canPtr, int *kalkan_kontrolPtr, Uint32 *kalkan_sayaciPtr);

#endif