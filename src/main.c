#include <SDL2/SDL.h>
#include <stdio.h>
#include "gemi.h"
#include "mermi.h"

int main(int argc, char *argv[])
{

    // sdl başlatıldı
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL baslatilamadi! Hata =  %s\n", SDL_GetError());
        return 1;
    }
    // pencere pointerı oluşturuldu ve kontrol edildi
    SDL_Window *window = SDL_CreateWindow("Asteroids - Uzay Macerasi", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          EKRAN_GENISLIK, EKRAN_YUKSEKLIK, 0);
    if (!window) { SDL_Quit(); return 1; }                                      
                                       
    // boyama işlemi için renderer oluşturuldu
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // oyun döngüsü kontrol değişkeni oluşturuldu
    int calisiyor = 1; 
    SDL_Event event; // olayları tutacak değişken

    struct Gemi uzaygemisi; // gemi structı oluşturuldu
    Mermi mermiler[MERMI_KAPASITE]; // mermi structı oluşturuldu

    gemi_baslangic(&uzaygemisi); // gemi başlangıç değerleri atandı ve konuma yerleştirildi
    mermi_baslangic(mermiler); // mermi başlangıç değerleri atandı

    const Uint8 *tuslar = SDL_GetKeyboardState(NULL); // klavye durumunu tutacak pointer oluşturuldu
    // oyun döngüsü 
    while (calisiyor) {
        while (SDL_PollEvent(&event)) {
            // pencerenin kapatma tuşuna basınca SDL_QUIT olayı tetiklenir
            if (event.type == SDL_QUIT) 
            {
                calisiyor = 0;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    mermi_atesleme(mermiler, &uzaygemisi);
                }
            }
        }
        gemi_kontrol(&uzaygemisi, tuslar); // klavyenin anlık durumunu kontrol etden ve haraketleri yöneten fonksiyon 

        gemi_hareket_et(&uzaygemisi); // bu fonksiyon bize geminin yeni konumunu güncelleyecek

        mermileri_guncelle(mermiler); // mermilerin konumunu güncellemek için fonksyonu çağırırız

        // ***------ EKRANA ÇİZME İŞLEMLERİ  -----***
        SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);  //arka plan rengi
        SDL_RenderClear(renderer); // ekranı boyar

        gemi_ciz(renderer, &uzaygemisi); // geminin son kordinatlarını ekrana çizer sadece yansıtmak kalır
        mermileri_ciz(renderer, mermiler); // mermilerin son kordinatlarını ekrana çizer sadece yansıtmak kalır

        //çizilen her şeyi ekrana yansıt
        SDL_RenderPresent(renderer);

        SDL_Delay(10); // oyun döngüsünün çok hızlı çalışmasını engellemek için kısa bir gecikme ekleyebiliriz yaklaşık 100 fps yapar bu şekilde
    }

    //açtığın şeyeleri kapat
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}