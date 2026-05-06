#ifndef MERMI_H
#define MERMI_H
#include <SDL2/SDL.h>
#include "gemi.h"

#define MERMI_GENISLIK 4
#define MERMI_YUKSEKLIK 15
#define MERMI_HIZ 10
#define MERMI_KAPASITE 10

typedef struct{
    float x;
    float y;
    float hizX;
    float hizY;
    int kontrol; // merminin erkanda olup olmadığını kontrol etmek için bir değişken
    SDL_Rect sekil;
} Mermi;

void mermi_baslangic(Mermi mermiler[]);
void mermi_atesleme(Mermi mermiler[], struct Gemi *gemiPtr);
void mermileri_guncelle(Mermi mermiler[]);
void mermileri_ciz(SDL_Renderer *renderer, Mermi mermiler[]);






#endif