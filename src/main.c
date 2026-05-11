#include <SDL2/SDL.h>
#include <stdio.h>
#include "gemi.h"
#include "mermi.h"
#include "asteroit.h"
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h> 
#include <SDL2/SDL_image.h>

// global değişkenler
TTF_Font *puan_font = NULL ;
TTF_Font *game_over_font = NULL ;
SDL_Window *window = NULL ;
SDL_Renderer *renderer = NULL ;
SDL_Surface *yaziYuzeyi = NULL ;
SDL_Texture *yaziDokusu = NULL ;
SDL_Surface *game_over_Yuzeyi = NULL ;
SDL_Texture *game_over_Dokusu = NULL ;
const Uint8 *tuslar = NULL;
SDL_Texture *gemi_Dokusu = NULL ;
SDL_Texture *asteroit_Dokusu1 = NULL ;
SDL_Texture *asteroit_Dokusu2 = NULL ;
SDL_Texture *asteroit_Dokusu3 = NULL ;

void baslat();
void puan_yazdir(int puan);
void game_over();

int main(int argc, char *argv[])
{
    srand(time(NULL));
    
    baslat();

    // oyun döngüsü kontrol değişkenleri oluşturuldu
    int puan = 0;
    int calisiyor = 1;
    int oyun_durdu = 0; // eğer bu değişken 1 olursa oyun güncellemeleri durur oyunu kapatmak yerine oyunu durdurmak daha mantıklı ekrana game over yazısı çıkarmak için 
    SDL_Event event; // olayları tutacak değişken

    //*********** NESNE OLUŞTURMA İŞLEMLERİ ***********
    struct Gemi uzaygemisi; // gemi structı oluşturuldu
    Mermi mermiler[MERMI_KAPASITE]; // mermi structı oluşturuldu
    Asteroit asteroitler[ASTEROIT_SAYISI]; // asteroit structı oluşturuldu

    // ********* NESNELERİ BAŞLATMA İŞLEMLERİ **********
    gemi_baslangic(&uzaygemisi); // gemi başlangıç değerleri atandı ve konuma yerleştirildi
    mermi_baslangic(mermiler); // mermi başlangıç değerleri atandı
    asteroit_baslangic(asteroitler); // asteroit başlangıç değerleri atandı

    // oyun döngüsü 
    while (calisiyor)
    {
        while (SDL_PollEvent(&event))
        {
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
        if(oyun_durdu == 0)
        {
            gemi_kontrol(&uzaygemisi, tuslar); // klavyenin anlık durumunu kontrol etden ve haraketleri yöneten fonksiyon 
            gemi_hareket_et(&uzaygemisi); // bu fonksiyon bize geminin yeni konumunu güncelleyecek
            mermileri_guncelle(mermiler); // mermilerin konumunu güncellemek için fonksyonu çağırırız

            //--------rastgele asteroit üretme işlemi ve çarpışma kontrol işlemleri -----------
            if(rand() % 70 == 0)
            {
                asteroit_uret(asteroitler);
            }
            asteroit_guncelle(asteroitler); // asteroitlerin konumunu güncellemek için fonksyonu çağırırız
            if(asteroit_carpisma_kontrol(asteroitler, &uzaygemisi) == 1)
            {
                printf("GAME OVER \nPuan = %d\n", puan);
                oyun_durdu = 1; // eğer çarğışma varsa oyunun döngüden çıkarır ve bitirir
            }
            if(asteroit_carpisma_kontrol_mermi(asteroitler , mermiler) == 1)
            {
                puan += 1;   // eğer çarpışma varsa puanı arttır
            }
            // güncelle kontrol et çiz şeklinde olmalı yoksa çok fazla lag oluyor astroid kısmında yaşadım bu problemi 
        }

            // ***------ EKRANA ÇİZME İŞLEMLERİ  -----***
            SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);  //arka plan rengi
            SDL_RenderClear(renderer); // ekranı boyar

            puan_yazdir(puan);
            if(oyun_durdu == 1)
                game_over();

            gemi_ciz(renderer, &uzaygemisi); // geminin son kordinatlarını ekrana çizer sadece yansıtmak kalır
            mermileri_ciz(renderer, mermiler); // mermilerin son kordinatlarını ekrana çizer sadece yansıtmak kalır
            asteroit_ciz(renderer, asteroitler); // asteroitlerin son kordinatlarını ekrana çizer sadece yansıtmak kalır


            
            //çizilen her şeyi ekrana yansıt
            SDL_RenderPresent(renderer);
            SDL_Delay(6);
        
    }
    
    //açtığın şeyeleri kapat sistem tasarrufu için
    if(gemi_Dokusu) {
        SDL_DestroyTexture(gemi_Dokusu);
    }
    IMG_Quit();
    TTF_CloseFont(puan_font);
    TTF_CloseFont(game_over_font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
void baslat()
{
    // sdl başlatıldı
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL baslatilamadi! Hata =  %s\n", SDL_GetError());
    }
    // font için ttf baslatıldı 
    if (TTF_Init() == -1) {
        printf("TTF hatasi: %s\n", TTF_GetError());
    }
    if (IMG_Init(IMG_INIT_PNG) == -1)
    {
        printf("Image hatasi: %s\n", IMG_GetError());
    }

    // font değeri atandı 
    puan_font = TTF_OpenFont("font.ttf", 24); // puan yazdırmak için font
    game_over_font = TTF_OpenFont("font.ttf", 60); // game over fontu için büyük bir font tanımladık 
    if (!puan_font)
    {
        printf("Font yuklenemedi: %s\n", TTF_GetError());
    }
    if (!game_over_font)
    {
        printf("Font yuklenemedi: %s\n", TTF_GetError());
    }
    // pencere pointerının içini dolduruyoruz
    window = SDL_CreateWindow("Asteroids - Uzay Macerasi", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          EKRAN_GENISLIK, EKRAN_YUKSEKLIK, 0);
    if (!window)
    {
        SDL_Quit();
    }  
    // boyama işlemi için rendererın içini dolduruyoruz
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    tuslar = SDL_GetKeyboardState(NULL); // klavye durumunu tutacak pointer oluşturuldu

    //gemi dokusu oluşturuldu png olarak kullanamıyoruz o yüzden texture olarak tanımlıyoruz
    gemi_Dokusu = IMG_LoadTexture(renderer, "C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\uzaygemisi.png");
    
    // asteroit dokuları 
    asteroit_Dokusu1 = IMG_LoadTexture(renderer, "C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\asteroit.png");
    asteroit_Dokusu2 = IMG_LoadTexture(renderer, "C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\asteroit2.png");
    asteroit_Dokusu3 = IMG_LoadTexture(renderer, "C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\asteroit3.png");
}
void puan_yazdir(int puan)
{
    char skorMetni[50];
    sprintf(skorMetni, "Skor: %d", puan);
    // burada bir string dizisi oluşturuyoruz ve puan değişkenini string veri tipine döndürür çünkü ekrana yazmak için string veri tipine ihtiyacımız var 
    
    SDL_Color beyaz = {255, 255, 255, 255}; 
    // beyaz adında bir değişten tutuyoruz veri tipi color olan 

    yaziYuzeyi = TTF_RenderText_Solid(puan_font , skorMetni, beyaz); //cpu tarafından tutulur ekrana çizilmesi çok yavaştır o yüzden doku oluşturuyoruz
    yaziDokusu = SDL_CreateTextureFromSurface(renderer, yaziYuzeyi);
    // burada da bir yüzey oluşturuyoruz ve sonra bu yüzeyden de doku oluşturuyoruz

    SDL_Rect yaziKutusu = {20, 20, yaziYuzeyi->w, yaziYuzeyi->h};
    // burada ekrana koyucağımız kutunun boyutunu ve konumunu ayarlıyoruz

    SDL_RenderCopy(renderer, yaziDokusu, NULL, &yaziKutusu);
    //burada da yazı dokusunu yaptığımız kutuya yapıştırıyoruz

    SDL_FreeSurface(yaziYuzeyi);
    SDL_DestroyTexture(yaziDokusu);
    // her döngüde yeni bir kutu oluştuğu için burada bunları kapatmamız gerekiyor
}
void game_over()
{
    SDL_Color kirmizi = {255, 0, 0, 255}; 
    // ekranın biraz kırmızı olması için  renk paleti tanımlıyoruz

    game_over_Yuzeyi = TTF_RenderText_Solid(puan_font ,"GAME OVER", kirmizi); //yazıyı bir yüzeye döndürüyoruz kızmı temalı 
    game_over_Dokusu = SDL_CreateTextureFromSurface(renderer, game_over_Yuzeyi);
    // burada da yüzeyi dokuya çeviriyoruz ekrana basabilmek için 

    SDL_Rect game_over_kutusu;
    game_over_kutusu.w = game_over_Yuzeyi->w;
    game_over_kutusu.h = game_over_Yuzeyi->h;
    game_over_kutusu.x = (EKRAN_GENISLIK - game_over_kutusu.w)/2;
    game_over_kutusu.y = (EKRAN_YUKSEKLIK - game_over_kutusu.h)/2;
    // burada ekranın tam ortasına bir kutu koyuyoruz

    SDL_RenderCopy(renderer, game_over_Dokusu, NULL, &game_over_kutusu);
    //burada da yazı dokusunu yaptığımız kutuya yapıştırıyoruz

    SDL_FreeSurface(game_over_Yuzeyi);
    SDL_DestroyTexture(game_over_Dokusu);
}