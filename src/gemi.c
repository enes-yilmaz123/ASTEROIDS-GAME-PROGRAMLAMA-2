#include "gemi.h"
#include <math.h>
#include <stdio.h>
extern SDL_Texture *gemi_Dokusu;
extern SDL_Texture *shield_supp_Dokusu;
extern SDL_Texture *aktif_gemi_Dokusu;

void gemi_baslangic(struct Gemi *gemiPtr)
{
    gemiPtr->x = (EKRAN_GENISLIK - GEMI_GENISLIK)/2;
    gemiPtr->y = (EKRAN_YUKSEKLIK - GEMI_YUKSEKLIK)/2;
    gemiPtr->sekil.w = GEMI_GENISLIK;
    gemiPtr->sekil.h = GEMI_YUKSEKLIK;
    gemiPtr->hiz_x= 0;
    gemiPtr->hiz_y= 0;
    gemiPtr->aci= 270;
}
void gemi_kontrol(struct Gemi *gemiPtr,const Uint8 *tuslar)
{
    float radyan;
    if (tuslar[SDL_SCANCODE_A])     // a tuşuna basılırsa geminin açısını azaltır
    {
        gemiPtr->aci -= GEMI_HIZ;
    }

    if (tuslar[SDL_SCANCODE_D])     // d tuşuna basılırsa geminin açısını azaltır
    {
        gemiPtr->aci += GEMI_HIZ;
    }

    if(gemiPtr->aci<0)      // geminin açısını 0 360 arasında tutmak için 
    gemiPtr->aci += 360;

    if(gemiPtr->aci>=360)
    gemiPtr->aci -= 360;

    if (tuslar[SDL_SCANCODE_W])
    {
        radyan = gemiPtr->aci*(M_PI/180);
        // aciyi radyana çeviriyoruz çünkü trogometrik fonksiyonlar kullanıcaz
        gemiPtr->hiz_x += cos(radyan) * 0.2; 
        gemiPtr->hiz_y += sin(radyan) * 0.2;
    }
    if (tuslar[SDL_SCANCODE_S])
    {
        radyan = gemiPtr->aci*(M_PI/180);
        // aciyi radyana çeviriyoruz çünkü trogometrik fonksiyonlar kullanıcaz
        gemiPtr->hiz_x -= cos(radyan) * 0.2; 
        gemiPtr->hiz_y -= sin(radyan) * 0.2;
    }



}
void gemi_hareket_et(struct Gemi *gemiPtr)
{
    // ilk  olarak geminin konumunu ivmesine göre güncelleriz
    gemiPtr->x += gemiPtr->hiz_x;
    gemiPtr->y += gemiPtr->hiz_y;

    gemiPtr->hiz_x *= 0.97;  // gemiyi ivmelendirdikten sonra geminin sonsuza kadar gitmemesi için sürtünme kuvveti
    gemiPtr->hiz_y *= 0.97;

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
void gemi_ciz(SDL_Renderer *renderer, struct Gemi *gemiPtr , int kalkan_kontrol) 
{
    //burada bir tane temp bir rect oluşturuyoruz bir sorun olursa elimizdeki gemi bozulmasın diye 
    SDL_Rect hedefKutu = gemiPtr->sekil;
    hedefKutu.h = 120;
    hedefKutu.w = 80;


    //bu fonksiyon elimizdeki dokuyu bir kutu üstüne yüklüyor
    SDL_RenderCopyEx(renderer, aktif_gemi_Dokusu, NULL, &gemiPtr->sekil, gemiPtr->aci+90, NULL, SDL_FLIP_NONE);
    //ilk parametre renderer 
    //2. eklemek istediğimiz resim 
    //3. resmin hepsini kopyalıyacak isek NULLyazıyoruz 
    //4.nereye kopyalıyacağını ve boyutunu belirtmesi için bir rect giriyoruz içine
    //5.dereceyi açıyı ayarladığımız yer bilgisayar ilk olarak otomatikmen sola bakarak çıkarıyor şekli de ona göre ayarlıyorz
    //6.resmin çevirilip çevirilmeyeceğine karar verir aynalama yapar
    

    if (kalkan_kontrol == 1)
    {
        // kalkanı koymak için bir kutu oluşturuyoruz
        SDL_Rect kalkan_kutusu =
        {
            gemiPtr->sekil.x - 10,
            gemiPtr->sekil.y - 10,
            gemiPtr->sekil.w + 20,
            gemiPtr->sekil.h + 20
        };
        //dokuyu kuyunun üzerine yüklüyoruz ve saydamlığını azaltıyoruz
        SDL_SetTextureAlphaMod(shield_supp_Dokusu, 180);
        SDL_RenderCopy(renderer, shield_supp_Dokusu, NULL, &kalkan_kutusu);
        
    }
}
