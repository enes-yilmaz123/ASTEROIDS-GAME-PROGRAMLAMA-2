#ifndef GEMI_H
#define GEMI_H
#include <SDL2/SDL.h>

// ekrn ve gemi boyutları
#define EKRAN_GENISLIK 1920
#define EKRAN_YUKSEKLIK 1080
#define GEMI_GENISLIK 100
#define GEMI_YUKSEKLIK 100
#define GEMI_HIZ 2.5

// gemi structı
struct Gemi {
    float x;
    float y;
    float hiz_x; // anlık hızı
    float hiz_y;   
    float aci; // açısını 0 ile 360 arasında bir değer olarak atıyoruz
    SDL_Rect sekil;
};

void gemi_baslangic(struct Gemi *gemiPtr);
void gemi_hareket_et(struct Gemi *gemiPtr);
void gemi_ciz(SDL_Renderer *renderer, struct Gemi *gemiPtr , int kalkan_kontrol);
void gemi_kontrol(struct Gemi *gemiPtr,const Uint8 *tuslar);
#endif