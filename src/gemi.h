#ifndef GEMI_H
#define GEMI_H
#include <SDL2/SDL.h>

// ekrn ve gemi boyutları
#define EKRAN_GENISLIK 1000
#define EKRAN_YUKSEKLIK 800
#define GEMI_GENISLIK 30
#define GEMI_YUKSEKLIK 60
#define GEMI_HIZ 3

// gemi structı
struct Gemi {
    float x;
    float y;
    float hizX;
    float hizY;
    SDL_Rect sekil;
    float yonX; //mermi ateşleme yönü için değişkenler
    float yonY;
};

void gemi_baslangic(struct Gemi *gemiPtr);
void gemi_hareket_et(struct Gemi *gemiPtr);
void gemi_ciz(SDL_Renderer *renderer, struct Gemi *gemiPtr);
void gemi_yon_degistir(struct Gemi *gemiPtr, float yeniYonX, float yeniYonY);

#endif