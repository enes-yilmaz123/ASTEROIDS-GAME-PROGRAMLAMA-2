#include "asteroit.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "gemi.h"

void asteroit_baslangic(Asteroit *asteroid) {
    for(int i = 0 ; i < ASTEROIT_SAYISI ;  i++)
    {
        asteroid[i].kontrol = 0;  //asteroidler başlandıçda pasif olmalı 
        asteroid[i].sekil.w = ASTEROIT_BOY;
        asteroid[i].sekil.h = ASTEROIT_BOY;

    }
}
void asteroit_uret(Asteroit *asteroit)
{
    for(int i = 0 ;  i < ASTEROIT_SAYISI ;  i++)
    {
        if(asteroit[i].kontrol == 0)  //pasif bir asteroid varsa onu aktif hale getir
        {
            asteroit[i].kontrol = 1; //asteroidi aktif hale getir
            asteroit[i].x = rand() % EKRAN_GENISLIK-ASTEROIT_BOY;  //asteroid x ve y konumlarını rastgele değerler ataadık 
            //asteroid[i].y = rand() % EKRAN_YUKSEKLIK-ASTEROIT_BOY;
            asteroit[i].y = -ASTEROIT_BOY;

            // -------- ASTEROİD HIZLARI ---------
            // astroidlerin hızlarını rastgele üreterek her birinin rotalarını da farklı yapabiliriz hemde hızları da farklı olur 

            asteroit[i].hizX = rand() % 5 - 2;
            asteroit[i].hizY = rand() % 3 + 2;

            break; //bir asteroid üretildikten sonra döngüden çık
        }
    }

}
void asteroit_guncelle(Asteroit *asteroit)
{
    for( int i = 0 ; i< ASTEROIT_SAYISI; i++)
    {
        if(asteroit[i].kontrol == 1 )
        {
            asteroit[i].x += asteroit[i].hizX;  // asteroidlerin hızına göre konumlarını güncelleme
            asteroit[i].y += asteroit[i].hizY;

            asteroit[i].sekil.x = asteroit[i].x;  //asteroid kordinatlarını sld_rect e atama yapıyoruz ki ekrana çizebilelim
            asteroit[i].sekil.y = asteroit[i].y;

            // asteroidleri silme işlemi ekran dışına çıkar asteroidleri pasif hale getiririz silmek için 

            if(asteroit[i].x <-ASTEROIT_BOY || asteroit[i].x > EKRAN_GENISLIK || asteroit[i].y < -ASTEROIT_BOY || asteroit[i].y > EKRAN_YUKSEKLIK)
            {
                asteroit[i].kontrol = 0; //asteroid pasif hale gelir
            }
        }
    }
}

void asteroit_ciz(SDL_Renderer *renderer, Asteroit *asteroit)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // asteroid için renk ayarı 

    for(int i = 0 ; i < ASTEROIT_SAYISI; i++)
    {
        if(asteroit[i].kontrol == 1) //aktif olan asteroidleri çiz
        {
            SDL_RenderFillRect(renderer, &asteroit[i].sekil);
        }
    }
}

int asteroit_carpisma_kontrol(Asteroit *asteroit, struct Gemi *gemiPtr)
{
    for(int i = 0 ; i < ASTEROIT_SAYISI; i++)
    {
        if(asteroit[i].kontrol == 1)
        {
            if(SDL_HasIntersection(&asteroit[i].sekil, &gemiPtr->sekil)) // bu fonksiyon iki tane nesnenin hitboxlarının kesişip kesişmediğini kontrol eder
            {                                                            // eğer kesişme varsa yani çarpışma varsa 1 döndürür yoksa 0
                return 1; // çarpışma var
            }
        }
    }
    return 0; // çarpışma yok
}