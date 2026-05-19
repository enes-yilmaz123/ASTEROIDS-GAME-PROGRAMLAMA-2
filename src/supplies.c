#include "supplies.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "gemi.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

extern SDL_Texture *repair_supp_Dokusu;
extern SDL_Texture *shield_supp_Dokusu;
extern Mix_Chunk *repair_efekti;

void supply_baslangic(Supply *supply)
{
    for(int i = 0 ; i < SUPPLY_MAX_SAYISI ;  i++)
    {
        supply[i].kontrol = 0;  //supplyler  başlandıçda pasif olmalı 
    }
}
void supply_uret(Supply *supply)
{
    for(int i = 0; i < SUPPLY_MAX_SAYISI; i++)
    {
        if(supply[i].kontrol == 0)
        {
            supply[i].sekil.x = rand() % (EKRAN_GENISLIK - SUPPLY_BOYUT);
            supply[i].sekil.y = rand() % (EKRAN_YUKSEKLIK - SUPPLY_BOYUT);
            supply[i].sekil.w = SUPPLY_BOYUT;
            supply[i].sekil.h = SUPPLY_BOYUT;
            supply[i].kontrol = 1;
            supply[i].tur = rand() % 2 + 1; // supply türünü rastgele atar 1 veya 2 olabilir
            
            break;
        }
    }
}
void supply_ciz(SDL_Renderer *renderer , Supply *supply)
{
    for(int i = 0 ; i < SUPPLY_MAX_SAYISI ; i++)
    {
        if(supply[i].kontrol == 1)
        {
            if(supply[i].tur == SUPPLY_REPAIR)
            {
                SDL_RenderCopyEx(renderer ,repair_supp_Dokusu,NULL,&supply[i].sekil,0,NULL,SDL_FLIP_NONE);
            }
            else if(supply[i].tur == SUPPLY_SHIELD)
            {
                SDL_RenderCopyEx(renderer ,shield_supp_Dokusu,NULL,&supply[i].sekil,0,NULL,SDL_FLIP_NONE);
            }
        }
    }
}
void supply_toplama_kontrol(Supply *supply, struct Gemi *gemiPtr, int *canPtr , int *kalkan_kontrolPtr, Uint32 *kalkan_sayaciPtr)
{
    for(int i = 0 ; i < SUPPLY_MAX_SAYISI; i++)
    {
        if(supply[i].kontrol == 1)
        {
            if(SDL_HasIntersection(&supply[i].sekil, &gemiPtr->sekil)) 
            {
                if(supply[i].tur == SUPPLY_REPAIR)
                {
                    *canPtr += 20;
                    if(*canPtr > 100)
                    {
                        *canPtr = 100;
                    }
                    supply[i].kontrol = 0;
                }
                if(supply[i].tur == SUPPLY_SHIELD)
                {
                    *kalkan_kontrolPtr = 1;                 // kalkan aktif etmek için 1 veriyoruz
                    *kalkan_sayaciPtr = SDL_GetTicks();     // o anki zamanı kaydet
                    supply[i].kontrol = 0;
                }
                Mix_PlayChannel(-1, repair_efekti, 0);
            }
        }
    }
}