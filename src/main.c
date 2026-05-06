#include <SDL2/SDL.h>
#include <stdio.h>
#include "gemi.h"
#include "mermi.h"

int main(int argc, char *argv[]) {

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
    if (!window) {
        SDL_Quit();
        return 1;
    }                                      
                                       
    // boyama işlemi için renderer oluşturuldu
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // oyun döngüsü kontrol değişkeni oluşturuldu
    int calisiyor = 1; 
    SDL_Event event; // olayları tutacak değişken

    struct Gemi uzaygemisi; // gemi structı oluşturuldu
    Mermi mermiler[MERMI_KAPASITE]; // mermi structı oluşturuldu

    gemi_baslangic(&uzaygemisi); // gemi başlangıç değerleri atandı ve konuma yerleştirildi
    mermi_baslangic(mermiler); // mermi başlangıç değerleri atandı
    // oyun döngüsü 
    while (calisiyor) {
        while (SDL_PollEvent(&event)) {
            // pencerenin kapatma tuşuna basınca SDL_QUIT olayı tetiklenir
            if (event.type == SDL_QUIT) 
            {
                calisiyor = 0;
            }
            else if(event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_w:
                        gemi_yon_degistir(&uzaygemisi,0,-1);
                        // w ye basıldığında yukarı haraket et ve yön değişkenini değiştir
                        break;
                    case SDLK_s:
                        gemi_yon_degistir(&uzaygemisi, 0, 1); // geminin yönünü aşağı yap
                        // s ye basıldığında aşağı haraket et ve yön değişkenini değiştir
                        break;
                    case SDLK_a:
                        gemi_yon_degistir(&uzaygemisi, -1, 0); // geminin yönünü sola yap
                        // a ya basıldığında sola haraket et ve yön değişkenini değiştir
                        break;
                    case SDLK_d:
                        gemi_yon_degistir(&uzaygemisi, 1, 0);
                        // d ye basıldığında sağa haraket et ve yön değişkenini değiştir
                        break;
                    case SDLK_SPACE: 
                    mermi_atesleme(mermiler, &uzaygemisi); 
                    break;
                }
            }
            else if(event.type == SDL_KEYUP)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_w:
                    case SDLK_s:
                        uzaygemisi.hizY = 0; // dikey hareket durdur
                        break;
                    case SDLK_a:
                    case SDLK_d:
                        uzaygemisi.hizX = 0; // yatay hareket durdur
                        break;
                }
            }
        }
        // geminin konumunu güncellemek için fonksyonu çağırırız
        gemi_hareket_et(&uzaygemisi); // bu fonksiyon bize geminin yeni konumunu güncelleyecek

        mermileri_guncelle(mermiler); // mermilerin konumunu güncellemek için fonksyonu çağırırız

        SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);  //arka plan rengi
        SDL_RenderClear(renderer); // ekranı boyar

        gemi_ciz(renderer, &uzaygemisi); // geminin son kordinatlarını ekrana çizer sadece yansıtmak kalır
        mermileri_ciz(renderer, mermiler); // mermilerin son kordinatlarını ekrana çizer sadece yansıtmak kalır

        //çizilen her şeyi ekrana yansıt
        SDL_RenderPresent(renderer);
    }

    //açtığın şeyeleri kapat
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}