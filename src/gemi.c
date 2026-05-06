#include "gemi.h"

void gemi_baslangic(struct Gemi *gemiPtr) {
    gemiPtr->x = (EKRAN_GENISLIK - GEMI_GENISLIK)/2;
    gemiPtr->y = (EKRAN_YUKSEKLIK - GEMI_YUKSEKLIK)/2;
    gemiPtr->hizX = 0;
    gemiPtr->hizY = 0;
    gemiPtr->sekil.w = GEMI_GENISLIK;
    gemiPtr->sekil.h = GEMI_YUKSEKLIK;
}

void gemi_hareket_et(struct Gemi *gemiPtr) {
    // ilk  olarak geminin konumunu hizina göre güncelleriz
    gemiPtr->x += gemiPtr->hizX;
    gemiPtr->y += gemiPtr->hizY;

    // sonrasında ekrandan çıkıp çıkmadığını kontrol ederiz
    if (gemiPtr->x > EKRAN_GENISLIK) 
    {
        gemiPtr->x = -GEMI_GENISLIK;
    }
    else if (gemiPtr->x < -GEMI_GENISLIK) 
    {
        gemiPtr->x = EKRAN_GENISLIK;
    }

    if (gemiPtr->y > EKRAN_YUKSEKLIK) 
    {
        gemiPtr->y = -GEMI_YUKSEKLIK;
    }
    else if (gemiPtr->y < -GEMI_YUKSEKLIK) 
    {
        gemiPtr->y = EKRAN_YUKSEKLIK;
    }
    // son olarak geminin kordinatlarını SDL_Rect yapısına atarız
    gemiPtr->sekil.x = (int)gemiPtr->x;
    gemiPtr->sekil.y = (int)gemiPtr->y;
}
// geminin kordinatları hazır geriye ekrana çizme kaldı
void gemi_ciz(SDL_Renderer *renderer, struct Gemi *gemiPtr) 
{
    //beyaz renk ayarlandı
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //ekrana gemi çizildi
    SDL_RenderFillRect(renderer, &gemiPtr->sekil);
    //ekranı yenileme işlemi main.c de yapılacak biz sadece geminin çizimini yazıyoru burada
}