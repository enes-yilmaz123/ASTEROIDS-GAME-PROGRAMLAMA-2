#ifndef ASTEROIT_H
#define ASTEROIT_H
#include <SDL2/SDL.h>
#include "gemi.h"
#include "mermi.h"

#define ASTEROIT_SAYISI 40

#define ASTEROIT_BUYUK_BOY 80
#define ASTEROIT_ORTA_BOY 60
#define ASTEROIT_KUCUK_BOY 40

#define ASTEROIT_MAX_HIZ 4
#define ASTEROIT_MIN_HIZ 1

typedef enum {
    BOYUT_KUCUK = 1,
    BOYUT_ORTA = 2,
    BOYUT_BUYUK = 3
} AsteroitBoyutu;

typedef struct {
    float x; 
    float y; 
    SDL_Rect sekil; 
    float hizX;
    float hizY;
    int kontrol;
    AsteroitBoyutu boyut;
    float aci;
    float donme_hizi;

}Asteroit;


void asteroit_baslangic(Asteroit *asteroit);
void asteroit_uret(Asteroit *asteroit);
void asteroit_guncelle(Asteroit *asteroit);
void asteroit_ciz(SDL_Renderer *renderer, Asteroit *asteroit);
void asteroit_parcala(Asteroit *asteroitler, float x, float y, AsteroitBoyutu boyut);
int asteroit_carpisma_kontrol(Asteroit *asteroit, struct Gemi *gemiPtr);
int asteroit_carpisma_kontrol_mermi(Asteroit *asteroit, Mermi *mermi);






#endif
