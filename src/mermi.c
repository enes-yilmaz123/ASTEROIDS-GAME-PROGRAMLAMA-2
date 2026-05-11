#include "mermi.h"
#include <math.h>
#include <SDL2/SDL_mixer.h>

extern Mix_Chunk *ates_efekti;

void mermi_baslangic(Mermi mermiler[]) 
{
    for (int i = 0; i < MERMI_KAPASITE; i++) 
    {
        mermiler[i].kontrol = 0;
        mermiler[i].hiz_x = 0;
        mermiler[i].hiz_y = 0;
        mermiler[i].sekil.w = MERMI_GENISLIK;
        mermiler[i].sekil.h = MERMI_YUKSEKLIK;
    }
}
void mermi_atesleme(Mermi mermiler[], struct Gemi *gemiPtr) {
    for (int i = 0; i < MERMI_KAPASITE; i++) 
    {
        if(mermiler[i].kontrol == 0)
        {
            mermiler[i].kontrol = 1;
            
            
            // başlangıç konumunu geminin merkezine yerleştiririz
            mermiler[i].x = gemiPtr->x + GEMI_GENISLIK/2;
            mermiler[i].y = gemiPtr->y + GEMI_YUKSEKLIK/2;

            float radyan = gemiPtr->aci * (M_PI / 180.0f);
            // geminin açısını radyana çeviriir
            
            mermiler[i].hiz_x = cos(radyan) * MERMI_HIZ;
            mermiler[i].hiz_y = sin(radyan) * MERMI_HIZ;
            
            // merminin kordinatlarını SDL_Rect yapısına atarız
            mermiler[i].sekil.x = (int)mermiler[i].x;
            mermiler[i].sekil.y = (int)mermiler[i].y;

            //ses efekti -1 ilk kanalı buluyor 0 ile 1 kere çalıyor döngüye sokmuyor
            Mix_PlayChannel(-1, ates_efekti, 0);
            
            break; 
        }
    }
}
void mermileri_guncelle(Mermi mermiler[]) 
{
    for (int i = 0; i < MERMI_KAPASITE; i++) 
    {
        if (mermiler[i].kontrol == 1) 
        {
            //merminin hızına göre konumunu güncelle
            mermiler[i].x += mermiler[i].hiz_x;
            mermiler[i].y += mermiler[i].hiz_y;
            
            // ekrandan çıkan mermileri silme
            if (mermiler[i].x <0|| mermiler[i].x>EKRAN_GENISLIK ||mermiler[i].y <0||mermiler[i].y>EKRAN_YUKSEKLIK)
            {
                mermiler[i].kontrol = 0;
            }
            mermiler[i].sekil.x = (int)mermiler[i].x;
            mermiler[i].sekil.y = (int)mermiler[i].y;
        }
    }
}
void mermileri_ciz(SDL_Renderer *renderer, Mermi mermiler[])
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // mermi rengini ayarla
    for (int i = 0; i < MERMI_KAPASITE; i++)
    {
        if (mermiler[i].kontrol == 1)
        {
            SDL_RenderFillRect(renderer, &mermiler[i].sekil);
        }
    }
}