#include <SDL2/SDL.h>
#include <stdio.h>

#define EKRAN_GENISLIK 1000
#define EKRAN_YUKSEKLIK 800
#define GEMI_GENISLIK 30
#define GEMI_YUKSEKLIK 40

int main(int argc, char *argv[]) {
    
    // sdl başlatıldı
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL baslatilamadi! Hata =  %s\n", SDL_GetError());
        return 1;
    }

    // pencere pointerı oluşturuldu
    SDL_Window *window = SDL_CreateWindow("Asteroids - Uzay Macerasi", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          EKRAN_GENISLIK, EKRAN_YUKSEKLIK, 0);
    
    // pencere kontrolü                                      
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
    
    SDL_Rect gemi; // gemi için dikdörtgen tanımlandı
    gemi.w = GEMI_GENISLIK;
    gemi.h = GEMI_YUKSEKLIK;

    float gemiX = (EKRAN_GENISLIK - GEMI_GENISLIK)/2; // geminin başlangıç konumu x ekseninde
    float gemiY = (EKRAN_YUKSEKLIK - GEMI_YUKSEKLIK)/2; 

    float gemiHizX = 0; // geminin başlangıç hızı x ekseninde
    float gemiHizY = 0; 

    // oyun döngüsü kontrol değişkeni oluşturuldu
    int calisiyor = 1; 
    SDL_Event event; // olayları tutacak değişken

    // oyun döngüsü 
    while (calisiyor) {

        while (SDL_PollEvent(&event)) {
            // pencerenin kapatma tuşuna basınca SDL_QUIT olayı tetiklenir
            if (event.type == SDL_QUIT) 
            {
                calisiyor = 0;
            }
            else if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym)
                {
                    case SDLK_w:
                        gemiHizY = -3; // yukarı hareket
                        break;
                    case SDLK_s:
                        gemiHizY = 3; // aşağı hareket
                        break;
                    case SDLK_a:
                        gemiHizX = -3; // sola hareket
                        break;
                    case SDLK_d:
                        gemiHizX = 3; // sağa hareket
                        break;
                }
            }
            else if(event.type == SDL_KEYUP){
                switch(event.key.keysym.sym)
                {
                    case SDLK_w:
                    case SDLK_s:
                        gemiHizY = 0; // dikey hareket durdur
                        break;
                    case SDLK_a:
                    case SDLK_d:
                        gemiHizX = 0; // yatay hareket durdur
                        break;
                }
            }
        }
        // geminin konumunu hızına göre güncelle
        gemiX += gemiHizX;
        gemiY += gemiHizY;

        //sağdan girerse soldan çıkar, soldan girerse sağdan çıkar
        if(gemiX > EKRAN_GENISLIK) {
            gemiX = -GEMI_GENISLIK;
        }
        else if(gemiX < -GEMI_GENISLIK) {
            gemiX = EKRAN_GENISLIK;
        }
        // aşağıdan girerse yukarı çıkar, yukarıdan girerse aşağı çıkar
        if(gemiY > EKRAN_YUKSEKLIK) {   
            gemiY = -GEMI_YUKSEKLIK;
        }
        else if(gemiY < -GEMI_YUKSEKLIK) {
            gemiY = EKRAN_YUKSEKLIK;
        }

        SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);  //arka plan rengi
        SDL_RenderClear(renderer); // ekranı boyar

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // gemi için beyaz renk

        //geminin kordinatlarını güncelle ve boyutlarını ayarla
        gemi.x = (int)gemiX; 
        gemi.y = (int)gemiY;

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