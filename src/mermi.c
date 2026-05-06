#include "mermi.h"

void mermi_baslangic(Mermi mermiler[]) 
{
    for (int i = 0; i < MERMI_KAPASITE; i++) 
    {
        mermiler[i].kontrol = 0;
        mermiler[i].hizX = 0;
        mermiler[i].hizY = 0;
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
            
            //merminin gideceği yöne göre şeklini belirleriz
            if(gemiPtr->yonY == 1 || gemiPtr->yonY == -1) // yukarı baktığında mermi dikey olur
            {
                mermiler[i].sekil.w = MERMI_GENISLIK;
                mermiler[i].sekil.h = MERMI_YUKSEKLIK;
            }
            else //sağa yada sola baktığında mermi yatay olur
            {
                mermiler[i].sekil.w = MERMI_YUKSEKLIK;
                mermiler[i].sekil.h = MERMI_GENISLIK;
            }

            // başlangıç konumunu geminin merkezine yerleştiririz
            mermiler[i].x = gemiPtr->x + (gemiPtr->sekil.w/2) - (mermiler[i].sekil.w/2);
            mermiler[i].y = gemiPtr->y + (gemiPtr->sekil.h/2) - (mermiler[i].sekil.h/2);
            
            // geminin yönünden yola çıkara merminin hızını belirleriz
            mermiler[i].hizX = gemiPtr->yonX * MERMI_HIZ;
            mermiler[i].hizY = gemiPtr->yonY * MERMI_HIZ;
            
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
            //merminin hızına göre konumunu güncelleriz haraket edebilsin diye
            mermiler[i].x += mermiler[i].hizX;
            mermiler[i].y += mermiler[i].hizY;
            
            mermiler[i].sekil.x = (int)mermiler[i].x;
            mermiler[i].sekil.y = (int)mermiler[i].y;

            // ekrandan çıkan mermileri silme
            if (mermiler[i].x <0|| mermiler[i].x>EKRAN_GENISLIK ||mermiler[i].y <0||mermiler[i].y>EKRAN_YUKSEKLIK)
            {
                mermiler[i].kontrol = 0;
            }
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