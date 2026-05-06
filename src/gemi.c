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
void gemi_kontrol(struct Gemi *gemiPtr,const Uint8 *tuslar)
{
    // tuşu bıraktığnda durması için hizını sıfırlıyoruz
    gemiPtr->hizX = 0;
    gemiPtr->hizY = 0;

    float YONX = 0;  // yön değişimi için geçici değişkenler
    float YONY = 0;

    //  -------------- GEMİNİN HAREKETİ İŞLEMLERİ ---------------------------

    if (tuslar[SDL_SCANCODE_W])     // w tuşuna basılırsa 1 döndürür ve gemi yukarı hareket eder
    {
        gemiPtr->hizY -= GEMI_HIZ;
        YONY -= 1;
    }
    if (tuslar[SDL_SCANCODE_S])     // s tuşuna basılırsa 1 döndürür ve gemi aşağı hareket eder
    {
        gemiPtr->hizY += GEMI_HIZ;
        YONY += 1;
    }
    if (tuslar[SDL_SCANCODE_A])     // a tuşuna basılırsa 1 döndürür ve gemi sola hareket eder
    {
        gemiPtr->hizX -= GEMI_HIZ;
        YONX -= 1;
    }
    if (tuslar[SDL_SCANCODE_D])     // d tuşuna basılırsa 1 döndürür ve gemi sağa hareket eder
    {
        gemiPtr->hizX += GEMI_HIZ;
        YONX += 1;
    }
    //  -------------- GEMİNİN YÖNÜNÜ DEĞİŞTİRME İŞLEMLERİ ---------------------------

    if (YONX != 0 || YONY != 0) // herhangi bir yön tuşuna basıldıysa
    {
        gemiPtr->yonX = YONX;
        gemiPtr->yonY = YONY;

        if (YONX != 0 && YONY == 0) // sadece sağ sol hareket varsa gemi yatay olur
        {
            gemiPtr->sekil.w = GEMI_YUKSEKLIK;
            gemiPtr->sekil.h = GEMI_GENISLIK;
        }

        else if (YONY != 0 && YONX == 0) // sadece yukarı aşağı hareket varsa gemi dikey olur
        {
            gemiPtr->sekil.w = GEMI_GENISLIK;
            gemiPtr->sekil.h = GEMI_YUKSEKLIK;
        }
    }
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
