#include <SDL2/SDL.h>
#include <stdio.h>


#define EKRAN_GENISLIK 1000
#define EKRAN_YUKSEKLIK 800
#define GEMI_GENISLIK 30
#define GEMI_YUKSEKLIK 40

int main(int argc, char *argv[]) {
    
    //sdl başlatıldı
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL baslatilamadi! Hata =  %s\n", SDL_GetError());
        return 1;
    }

    // pencere pointerı oluşturuldu
    SDL_Window *window = SDL_CreateWindow("Asteroids - Uzay Macerasi", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          EKRAN_GENISLIK, EKRAN_YUKSEKLIK, 0);
    
    //pencere kontrolü                                      
    if (!window) {
        printf("Pencere olusturulamadi Hata = %s \n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // boyama işlemi için renderer oluşturuldu ve kontrol edildi
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Renderer olusturulamadi Hata = %s \n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // oyun döngüsü kontrol değişkeni oluşturuldu
    int calisiyor = 1; 
    SDL_Event event; // olayları tutacak değişken

    // oyun döngüsü 
    while (calisiyor) {


        while (SDL_PollEvent(&event)) {
            // pencerenin kapatma tuşuna basınca SDL_QUIT olayı tetiklenir
            if (event.type == SDL_QUIT) {
                calisiyor = 0;
            }
        }


        SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);  //arka plan rengi
        SDL_RenderClear(renderer); // ekranı boyar

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // gemi için beyaz renk

        SDL_Rect gemi; // gemi için dikdörtgen tanımlandı

        gemi.x = (EKRAN_GENISLIK - GEMI_GENISLIK)/2; // gemiyi ekranın ortasına yerleştir
        gemi.y = (EKRAN_YUKSEKLIK - GEMI_YUKSEKLIK)/2;
        gemi.w = GEMI_GENISLIK;
        gemi.h = GEMI_YUKSEKLIK;

        SDL_RenderFillRect(renderer, &gemi); // gemiyi çiz


        //çizilen her şeyi ekrana yansıt
        SDL_RenderPresent(renderer);
    }

    //açtığın şeyeleri kapat
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}