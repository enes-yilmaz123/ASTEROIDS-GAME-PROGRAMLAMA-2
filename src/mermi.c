#include "mermi.h"
#include <math.h>
#include <SDL2/SDL_mixer.h>

extern Mix_Chunk *ates_efekti;
extern SDL_Texture *mermi_Dokusu;

void mermi_baslangic(Mermi mermiler[]) 
{
    for (int i = 0; i < MERMI_KAPASITE; i++) 
    {
        mermiler[i].kontrol = 0;
        mermiler[i].hiz_x = 0;
        mermiler[i].hiz_y = 0;
        mermiler[i].sekil.w = MERMI_GENISLIK;
        mermiler[i].sekil.h = MERMI_YUKSEKLIK;
        mermiler[i].aci = 0;
    }
}
void mermi_atesleme(Mermi mermiler[], struct Gemi *gemiPtr) {
    for (int i = 0; i < MERMI_KAPASITE; i++) 
    {
        if(mermiler[i].kontrol == 0)
        {
            mermiler[i].kontrol = 1;
            
            // başlangıç konumunu geminin merkezine yerleştiririz
            mermiler[i].x = gemiPtr->x + (GEMI_GENISLIK/2)-12;
            mermiler[i].y = gemiPtr->y + GEMI_YUKSEKLIK/2;

            float radyan = gemiPtr->aci * (M_PI / 180);
            // geminin açısını radyana çeviriir
            
            mermiler[i].hiz_x = cos(radyan) * MERMI_HIZ;
            mermiler[i].hiz_y = sin(radyan) * MERMI_HIZ;

            mermiler[i].aci = gemiPtr->aci;
            
            // merminin kordinatlarını SDL_Rect yapısına atarız
            mermiler[i].sekil.x = (int)mermiler[i].x;
            mermiler[i].sekil.y = (int)mermiler[i].y;

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
    for(int i = 0 ; i < MERMI_KAPASITE ; i++)
    {
        if(mermiler[i].kontrol == 1)
        {
            SDL_RenderCopyEx(renderer ,mermi_Dokusu,NULL,&mermiler[i].sekil,mermiler[i].aci,NULL,SDL_FLIP_NONE);
        }
    }
}