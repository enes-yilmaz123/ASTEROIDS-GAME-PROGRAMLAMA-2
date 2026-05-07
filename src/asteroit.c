#include "asteroit.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "gemi.h"
#include "mermi.h"

void asteroit_baslangic(Asteroit *asteroit) {
    for(int i = 0 ; i < ASTEROIT_SAYISI ;  i++)
    {
        asteroit[i].kontrol = 0;  //asteroidler başlandıçda pasif olmalı 
    }
}
void asteroit_uret(Asteroit *asteroit)
{
    for(int i = 0 ;  i < ASTEROIT_SAYISI ;  i++)
    {
        if(asteroit[i].kontrol == 0)  //pasif bir asteroid varsa onu aktif hale getir
        {
            asteroit[i].kontrol = 1; //asteroidi aktif hale getir
            
            int gercek_boyut; 
            asteroit[i].boyut = rand() % 3 + 1;
            if(asteroit[i].boyut == BOYUT_BUYUK)
            gercek_boyut = ASTEROIT_BUYUK_BOY;

            else if(asteroit[i].boyut == BOYUT_ORTA)
            gercek_boyut = ASTEROIT_ORTA_BOY;

            else
            gercek_boyut = ASTEROIT_KUCUK_BOY;
            //burada urettigimiz rastgele değerleri bir temp değiskenine atıp sonra da o temp değişkenine pixel boyutlarını atıyoruz

            asteroit[i].sekil.w = gercek_boyut;
            asteroit[i].sekil.h = gercek_boyut;


            asteroit[i].x = rand() % (EKRAN_GENISLIK - gercek_boyut);  //asteroid x ve y konumlarını rastgele değerler ataadık 
            asteroit[i].y = -gercek_boyut; //asteroidlerin ekranın üstünden gelmesi için y konumunu ekanın üstüne başlatıyoruz

            // -------- ASTEROİD HIZLARI ---------
            // astroidlerin hızlarını rastgele üreterek her birinin rotalarını da farklı yapabiliriz hemde hızları da farklı olur 

            asteroit[i].hizX = rand() % 5 - 2;
            asteroit[i].hizY = rand() % 3 + 2;

            break; //bir asteroid üretildikten sonra döngüden çık
        }
    }
}
void asteroit_parcala(Asteroit *asteroitler, float x, float y, AsteroitBoyutu boyut) {
    int uretilen = 0; // kaç tane yeni parça ürettiğimizi saymak için bir sayaç
    
    for(int i = 0; i < ASTEROIT_SAYISI; i++)
    {
        if(asteroitler[i].kontrol == 0) // boşda bir asteroid varsa yeni asteroid parçalarını oraya yerleştir 
        { 
            asteroitler[i].kontrol = 1; // boş asteroid aktif hale getir
            asteroitler[i].boyut = boyut; // yeni parçaların boyutunu parçalanan taşın boyutuna göre ayarla
            
            // pixel boyutlarını belirle
            int gercekBoy;
            if(boyut == BOYUT_ORTA)
            {
                gercekBoy = ASTEROIT_ORTA_BOY;
            }
            else
            {
                gercekBoy = ASTEROIT_KUCUK_BOY;
            }

            asteroitler[i].sekil.w = gercekBoy; // bu boyutları asteroit structına atıyoruz ki ekrana çizebilelim
            asteroitler[i].sekil.h = gercekBoy;
            
            // eski taşın parçalandığı yere diğer taşları yerleştir
            asteroitler[i].x = x;
            asteroitler[i].y = y;

            // parçaların dağılması için ters yönlerde hızlar verelim
            if(uretilen == 0)
            {
                asteroitler[i].hizX = -2;
            }
            else 
            {
                asteroitler[i].hizX = 2;
            }
            uretilen++;
            if(uretilen == 2)
            {
                break; // iki parçayı da ürettikten sonra döngüden çkı
            }
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
            
            // spawnlandığı gibi silinmemesi için biraz daha fazla sınır koydum 
            if(asteroit[i].x < -100 || asteroit[i].x > EKRAN_GENISLIK + 100 || asteroit[i].y < -150 || asteroit[i].y > EKRAN_YUKSEKLIK + 50)
            {
                asteroit[i].kontrol = 0; 
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
int asteroit_carpisma_kontrol_mermi(Asteroit *asteroit, Mermi *mermi)
{
    int sayac = 0; // kaç tane çarpışma olduğunu saymak için bir sayaç
    for(int i = 0 ; i <ASTEROIT_SAYISI; i++)
    {
        if(asteroit[i].kontrol == 1)
        {
            for(int j = 0 ; j < MERMI_KAPASITE; j++)
            {
                if(mermi[j].kontrol == 1)
                {
                    if(SDL_HasIntersection(&asteroit[i].sekil, &mermi[j].sekil))
                    {
                        // mermi çarpan asteroitin boyutuna göre atama yap pixel olarak ve ardından parçala fonksiyonunu çağırarak olduğu yere daha küçük boyutta iki parça gönderiyoruz
                        if(asteroit[i].boyut == BOYUT_BUYUK)
                        {
                            asteroit_parcala(asteroit, asteroit[i].x, asteroit[i].y, BOYUT_ORTA);
                        }

                        else if(asteroit[i].boyut == BOYUT_ORTA)
                        {
                            asteroit_parcala(asteroit, asteroit[i].x, asteroit[i].y, BOYUT_KUCUK);
                        }
                        // küçükleri direkt yok ediyoruz

                        // vurulan eski asteroiti ve mermiyi yok et
                        asteroit[i].kontrol = 0; 
                        mermi[j].kontrol = 0; 
                        
                        // vurduğun her asteroit için puan kazanırısn 
                        sayac += 1; 
                        
                        break;
                    }
                }
            }
        }
    }
    return sayac; // kaç tane çarpışma olduğunu döndürür
}
