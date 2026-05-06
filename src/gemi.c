#include "gemi.h"

void gemi_baslangic(struct Gemi *gemiPtr) {
    gemiPtr->x = (EKRAN_GENISLIK - GEMI_GENISLIK)/2;
    gemiPtr->y = (EKRAN_YUKSEKLIK - GEMI_YUKSEKLIK)/2;
    gemiPtr->hizX = 0;
    gemiPtr->hizY = 0;
    gemiPtr->yonX = 0;
    gemiPtr->yonY = -1; // başlangıçta yukarı bakacak şekilde yön verilir
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
void gemi_yon_degistir(struct Gemi *gemiPtr, float yeniYonX, float yeniYonY) 
{
    gemiPtr->yonX = yeniYonX; //geminin yönünü temp olarak tutarız
    gemiPtr->yonY = yeniYonY;

    gemiPtr->hizX = yeniYonX * GEMI_HIZ;   //geminin hızını yeni yönüne göre güncelleriz
    gemiPtr->hizY = yeniYonY * GEMI_HIZ;

    if(gemiPtr->yonY == 1 || gemiPtr->yonY == -1)
    {
        gemiPtr->sekil.w = GEMI_GENISLIK; //geminin yönüne göre şeklini belirleriz
        gemiPtr->sekil.h = GEMI_YUKSEKLIK;
    }
    else
    {
        gemiPtr->sekil.w = GEMI_YUKSEKLIK;
        gemiPtr->sekil.h = GEMI_GENISLIK;
    }
}