#include "asteroit.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "gemi.h"
#include "mermi.h"
#include <math.h>
#include <SDL2/SDL_mixer.h>
extern Mix_Chunk *patlama_efekti;
extern SDL_Texture *asteroit_Dokusu1;
extern SDL_Texture *asteroit_Dokusu2;
extern SDL_Texture *asteroit_Dokusu3;

void asteroit_baslangic(Asteroit *asteroit) {
    for(int i = 0 ; i < ASTEROIT_SAYISI ;  i++)
    {
        asteroit[i].kontrol = 0;  //asteroidler başlandıçda pasif olmalı 
        asteroit[i].aci = 0;
    }
}
void asteroit_uret(Asteroit *asteroit)
{
    int buyuk_sayac = 0;
    int orta_sayac = 0;
    for(int i = 0; i < ASTEROIT_SAYISI; i++)
    {
        if(asteroit[i].kontrol == 1) //aktif asteroidleri say
        {
            if(asteroit[i].boyut == BOYUT_BUYUK)
            buyuk_sayac++;
            else if(asteroit[i].boyut == BOYUT_ORTA)
            orta_sayac++;
        }
    }
    if(buyuk_sayac < 7 && orta_sayac < 10) // eğer ekranda yeterince büyük ve orta boyutlu asteroid varsa
    {
        for(int i = 0 ;  i < ASTEROIT_SAYISI ;  i++)
        {
            if(asteroit[i].kontrol == 0)  //pasif bir asteroid varsa onu aktif hale getir
            {
                asteroit[i].kontrol = 1; //asteroidi aktif hale getir
            
                int gercek_boyut; 
                asteroit[i].boyut = rand() % 2 + 2;
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
            
                if(asteroit[i].boyut == BOYUT_KUCUK)
                {
                    asteroit[i].donme_hizi = 0; //en küçük boylu meteor kayan yıldız şeklinde olduğu için dönmesini istemiyoruz
                
                    // gittiği yönün açısını radyan cinsinden bulup dereceye çeviriyoruz
                    float radyan = atan2(asteroit[i].hizY, asteroit[i].hizX);
                    float gercek_aci = radyan * (180/M_PI);
                
                    asteroit[i].aci = gercek_aci - 45; // resimde meteor sağ alta doğru baktığı için onu bilgisayarların başlangıç kordinatları ile eşliyoruz yani sağa bakıtıyoruz
                }
                else 
                {
                    // orta ve büyük asteroitlerin dönmesini istiyorum o yüzden rastgele bir açı değeri atadım
                    asteroit[i].aci = rand() % 360; 
                    asteroit[i].donme_hizi = (rand() % 5) - 2; 
                }
            
                break; //bir asteroid üretildikten sonra döngüden çık
            }
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
            asteroitler[i].hizY = 2; // aşağıya doğru da hız veriyoruz

            if (boyut == BOYUT_KUCUK) 
            {
                //küçük asteroitler dönmesin
                asteroitler[i].donme_hizi = 0;
                
                // gittiği yönün açısını radyan cinsinden bulup dereceye çeviriyoruz
                float radyan = atan2(asteroitler[i].hizY, asteroitler[i].hizX);
                asteroitler[i].aci = (radyan * (180/M_PI))-45; // resmi hizzalamak için 45 derece çıkartıyoruz

            } 
            else 
            {
                //orta asteroitler dönsün 
                asteroitler[i].aci = rand() % 360; 
                asteroitler[i].donme_hizi = (rand() % 5) - 2; 
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

            asteroit[i].aci += asteroit[i].donme_hizi;
            if (asteroit[i].aci >= 360) asteroit[i].aci -= 360;
            if (asteroit[i].aci < 0) asteroit[i].aci += 360;

            if (asteroit[i].x > EKRAN_GENISLIK) 
            {
                asteroit[i].x = -GEMI_GENISLIK;
            }
            else if (asteroit[i].x < -GEMI_GENISLIK) 
            {
                asteroit[i].x = EKRAN_GENISLIK;
            }
            if (asteroit[i].y > EKRAN_YUKSEKLIK) 
            {
                asteroit[i].y = -GEMI_YUKSEKLIK;
            }
            else if (asteroit[i].y < -GEMI_YUKSEKLIK) 
            {
                asteroit[i].y = EKRAN_YUKSEKLIK;
            }

            asteroit[i].sekil.x = asteroit[i].x;  //asteroid kordinatlarını sld_rect e atama yapıyoruz ki ekrana çizebilelim
            asteroit[i].sekil.y = asteroit[i].y;

        }
    }
}
void asteroit_ciz(SDL_Renderer *renderer, Asteroit *asteroit)
{
    for(int i = 0; i< ASTEROIT_SAYISI; i++)
    {
        if(asteroit[i].kontrol == 1) //aktif olan asteroidleri çiz
        {
            if(asteroit[i].boyut == BOYUT_BUYUK)
            {
                SDL_RenderCopyEx(renderer ,asteroit_Dokusu1,NULL,&asteroit[i].sekil,asteroit[i].aci,NULL,SDL_FLIP_NONE);
            }
            else if(asteroit[i].boyut == BOYUT_ORTA)
            {
                SDL_RenderCopyEx(renderer ,asteroit_Dokusu2,NULL,&asteroit[i].sekil,asteroit[i].aci,NULL,SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderCopyEx(renderer ,asteroit_Dokusu3,NULL,&asteroit[i].sekil,asteroit[i].aci,NULL,SDL_FLIP_NONE);
            }

        }
    }
    
}
int asteroit_carpisma_kontrol(Asteroit *asteroit, struct Gemi *gemiPtr , int *canPtr , int *kalkanPtr, Uint32 *sayacPtr)
{
    if (*kalkanPtr == 1) 
    {
        if (SDL_GetTicks() - *sayacPtr > 3000)
        {
            *kalkanPtr = 0; // 3 saniye sonra kalkanı pasfi hale getir
        }
        else
        {
            return 0; // kalkan aktif ise direkt çık
        }
    }
    int kontrol = 0;
    SDL_Rect geciciGemi;
    geciciGemi.w = gemiPtr->sekil.w-10;
    geciciGemi.h = gemiPtr->sekil.h+10;

    geciciGemi.x = gemiPtr->sekil.x; 
    geciciGemi.y = gemiPtr->sekil.y + 10;

    for(int i = 0 ; i < ASTEROIT_SAYISI; i++)
    {
        if(asteroit[i].kontrol == 1)
        {
            if(SDL_HasIntersection(&asteroit[i].sekil, &geciciGemi)) // bu fonksiyon iki tane nesnenin hitboxlarının kesişip kesişmediğini kontrol eder
            {                                                          // eğer kesişme varsa yani çarpışma varsa 1 döndürür yoksa 0
                if(asteroit[i].boyut == BOYUT_BUYUK)
                {
                    *canPtr -= 30;
                    kontrol = -30;
                }
                else if(asteroit[i].boyut == BOYUT_ORTA)
                {
                    *canPtr -= 20;
                    kontrol = -20;
                }
                else
                {
                    *canPtr -= 10;
                    kontrol = -10;
                }
                asteroit[i].kontrol = 0 ;
                
                break;
            }
        }
    }
    return kontrol ;
}
int asteroit_carpisma_kontrol_mermi(Asteroit *asteroit, Mermi *mermi)
{
    int skor = 0;
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
                        
                        //küçükleri direkt yok ediyoruz

                        //vurulan eski asteroiti ve mermiyi yok et
                        asteroit[i].kontrol = 0; 
                        mermi[j].kontrol = 0; 
                        
                        //vurduğun her asteroit için puan kazanırısn 
                        skor = 1; 
                        
                        //ses efekti eklendi -1 en uygun kanalı bulur 0 ise tek sefer çalmasını sağlar
                        Mix_PlayChannel(-1, patlama_efekti, 0);
                        
                        break;
                    }
                }
            }
        }
    }
    return skor; // kaç tane çarpışma olduğunu döndürür
}
