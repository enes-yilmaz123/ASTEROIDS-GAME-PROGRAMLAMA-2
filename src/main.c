#include <SDL2/SDL.h>
#include <stdio.h>
#include "gemi.h"
#include "mermi.h"
#include "asteroit.h"
#include "supplies.h"
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// global değişkenler
TTF_Font *puan_font = NULL ;
TTF_Font *game_over_font = NULL ;
TTF_Font *menu_ekrani_font = NULL ;

SDL_Window *window = NULL ;
SDL_Renderer *renderer = NULL ;

SDL_Texture *mermi_Dokusu = NULL ;
SDL_Surface *yaziYuzeyi = NULL ;
SDL_Texture *yaziDokusu = NULL ;
SDL_Texture *gemi_Dokusu = NULL ;
SDL_Texture *asteroit_Dokusu1 = NULL ;
SDL_Texture *asteroit_Dokusu2 = NULL ;
SDL_Texture *asteroit_Dokusu3 = NULL ;
SDL_Texture *arkaPlan_Dokusu = NULL ;
SDL_Texture *repair_supp_Dokusu = NULL ;
SDL_Texture *shield_supp_Dokusu = NULL ;

Mix_Music *arkaPlanMuzigi = NULL;
Mix_Chunk *ates_efekti = NULL;
Mix_Chunk *patlama_efekti = NULL;
Mix_Chunk *respawn_efekti = NULL;
Mix_Chunk *click_efekti = NULL;
Mix_Chunk *hasar_efekti = NULL;
Mix_Chunk *repair_efekti = NULL;

const Uint8 *tuslar = NULL;

SDL_Color beyaz = {255,255,255,255};
SDL_Color mavi = {0,0,255,255};
SDL_Color kirmizi = {255,0,0,255};
SDL_Color yesil = {0,255,0,255};

typedef enum
{
    DURUM_MENU,DURUM_OYUNDA,DURUM_GAMEOVER,DURUM_AYARLAR
} OyunDurumu;
typedef struct
{
    SDL_Rect sekil;
    char *metin;
}Buton;

Buton basla_butonu;
Buton cikis_butonu;
Buton ayarlar_butonu;
Buton tekrar_oyna_butonu;
Buton menu_butonu;
Buton geri_don_butonu;
Buton ses_arttir_butonu;
Buton ses_azalt_butonu;

void baslat();
void puan_yazdir(int puan);
void game_over(int puan, int *personalBest);
void menu_ekrani_ciz(int *personalBest);
void ekrana_yazi_yaz(const char *metin, int x, int y, TTF_Font *secilen_font, SDL_Color renk);
void yaziyi_ortala_ciz(char *metin, int y_kordinati, TTF_Font *font, SDL_Color renk);
void can_bar_ciz(int can);
void buton_yazdir(SDL_Renderer *renderer, Buton buton, TTF_Font *font);
int buton_tiklama_kontrol(int fare_x, int fare_y, Buton buton);
void ayarlar_menusu_ciz();

int main(int argc, char *argv[])
{
    int puan = 0;
    int can = 100;
    int menu_kontrol = 0;
    int personalBest = 0;
    int kalkan_kontrol = 0;
    Uint32 kalkan_sayaci = 0;
    
    srand(time(NULL));
    baslat();
    
    //oyun ilk açıldığında menü durumunda olması için
    OyunDurumu anlik_durum = DURUM_MENU;

    // oyun döngüsü kontrol değişkenleri oluşturuldu
    int calisiyor = 1;

    SDL_Event event; // olayları tutacak değişken

    //*********** NESNE OLUŞTURMA İŞLEMLERİ ***********
    struct Gemi uzaygemisi; // gemi structı oluşturuldu
    Mermi mermiler[MERMI_KAPASITE]; // mermi structı oluşturuldu
    Asteroit asteroitler[ASTEROIT_SAYISI]; // asteroit structı oluşturuldu
    Supply supplies[SUPPLY_MAX_SAYISI]; // supply structı oluşturuldu

    // ********* NESNELERİ BAŞLATMA İŞLEMLERİ **********
    gemi_baslangic(&uzaygemisi); // gemi başlangıç değerleri atandı ve konuma yerleştirildi
    mermi_baslangic(mermiler); // mermi başlangıç değerleri atandı
    asteroit_baslangic(asteroitler); // asteroit başlangıç değerleri atandı
    supply_baslangic(supplies); // supply başlangıç değerleri atandı

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
            if (event.type == SDL_MOUSEBUTTONDOWN)  // fare tıklama olayı için gerekli işlemler
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int fare_x = event.button.x;
                    int fare_y = event.button.y;

                    if(anlik_durum == DURUM_AYARLAR) // eğer oyun ayarlar durumundaysa
                    {
                        if(buton_tiklama_kontrol(fare_x, fare_y, geri_don_butonu))
                        {
                            Mix_PlayChannel(-1, click_efekti, 0);
                            if(menu_kontrol == 1)
                            {
                                anlik_durum = DURUM_OYUNDA;
                                menu_kontrol = 0;
                            }
                            else
                            {
                                anlik_durum = DURUM_MENU;
                            }
                        }
                        if(buton_tiklama_kontrol(fare_x, fare_y, ses_arttir_butonu))
                        {
                            int anlik_ses = Mix_VolumeMusic(-1); //şu anki ses seviyesini öğren
                            anlik_ses += 13; // sesi %10 arttır
                            
                            if(anlik_ses > 128) anlik_ses = 128; // maks ses seviyesi 128 onu geçmemesi lazım
                            
                            //tüm seslere yeni sesi uygula
                            Mix_VolumeMusic(anlik_ses);
                            Mix_VolumeChunk(ates_efekti, anlik_ses);
                            Mix_VolumeChunk(patlama_efekti, anlik_ses-50);
                            Mix_VolumeChunk(respawn_efekti, anlik_ses);
                            Mix_VolumeChunk(hasar_efekti, anlik_ses);
                        }
                        if(buton_tiklama_kontrol(fare_x, fare_y, ses_azalt_butonu))
                        {
                            int anlik_ses = Mix_VolumeMusic(-1); //şu anki ses seviyesini öğren
                            anlik_ses -= 13; //%10 azalt
                            
                            if(anlik_ses < 0) anlik_ses = 0;// 0 dan aşağı düşmesin
                            
                            Mix_VolumeMusic(anlik_ses);
                            Mix_VolumeChunk(ates_efekti, anlik_ses);
                            Mix_VolumeChunk(patlama_efekti, anlik_ses-50);
                            Mix_VolumeChunk(respawn_efekti, anlik_ses);
                            Mix_VolumeChunk(hasar_efekti, anlik_ses);
                        }
                    }
                    if(anlik_durum == DURUM_MENU) // eğer oyun menü durumundaysa
                    { 
                        if (buton_tiklama_kontrol(fare_x, fare_y, basla_butonu))
                        {
                            Mix_PlayChannel(-1, click_efekti, 0);
                            anlik_durum = DURUM_OYUNDA;
                        }
                        else if (buton_tiklama_kontrol(fare_x, fare_y, cikis_butonu))
                        {
                            Mix_PlayChannel(-1, click_efekti, 0);
                            SDL_Delay(300); // çıkış efektinin duyulamsı için ufak  bir gecikme
                            calisiyor = 0;
                        }
                        else if (buton_tiklama_kontrol(fare_x, fare_y, ayarlar_butonu))
                        {
                            Mix_PlayChannel(-1, click_efekti, 0);
                            anlik_durum = DURUM_AYARLAR;
                        }
                    }
                    if(anlik_durum == DURUM_GAMEOVER) // eğer oyun game over durumundaysa
                    {
                        if(buton_tiklama_kontrol(fare_x, fare_y, cikis_butonu))
                        {
                            Mix_PlayChannel(-1, click_efekti, 0);
                            calisiyor = 0;
                        }
                        if(buton_tiklama_kontrol(fare_x, fare_y, tekrar_oyna_butonu))
                        {
                            Mix_PlayChannel(-1, click_efekti, 0);
                            puan = 0;
                            can = 100;
                            gemi_baslangic(&uzaygemisi); // gemi başlangıç değerleri atandı ve konuma yerleştirildi
                            mermi_baslangic(mermiler); // mermi başlangıç değerleri atandı
                            asteroit_baslangic(asteroitler); // asteroit başlangıç değerleri atandı
                            supply_baslangic(supplies); // supply başlangıç değerleri atandı
                            anlik_durum = DURUM_OYUNDA;
                        }
                        if(buton_tiklama_kontrol(fare_x, fare_y, menu_butonu))
                        {
                            Mix_PlayChannel(-1, click_efekti, 0);
                            puan = 0;
                            can = 100;
                            gemi_baslangic(&uzaygemisi); // gemi başlangıç değerleri atandı ve konuma yerleştirildi
                            mermi_baslangic(mermiler); // mermi başlangıç değerleri atandı
                            asteroit_baslangic(asteroitler); // asteroit başlangıç değerleri atandı
                            supply_baslangic(supplies); // supply başlangıç değerleri atandı
                            anlik_durum = DURUM_MENU;
                        }
                    }
                }
            }
            if (event.type == SDL_KEYDOWN) // tuşa basılma durmu gerçekleştiyse
            {
                if(anlik_durum == DURUM_OYUNDA) // if oyun durumu oyunda ise 
                {
                    if (event.key.keysym.sym == SDLK_SPACE) // space tuşuna basılınca eteş et fonksiyonunu çalıştır ve ses gelsin 
                    {
                        mermi_atesleme(mermiler, &uzaygemisi);
                        //ses efekti -1 ilk kanalı buluyor 0 ile 1 kere çalıyor döngüye sokmuyor
                        Mix_PlayChannel(-1, ates_efekti, 0);
                    }
                    if (event.key.keysym.sym == SDLK_ESCAPE) // esc tuşuna basılınca ayarlara git
                    {
                        Mix_PlayChannel(-1, click_efekti, 0);
                        menu_kontrol = 1;
                        anlik_durum = DURUM_AYARLAR;
                    }
                }
            }
        }
        // arka planı çizim ilemlerinden önce koyarız
        SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);  //arka plan rengi
        SDL_RenderClear(renderer); // ekranı boyar

        if(anlik_durum == DURUM_MENU)
        {
            SDL_RenderCopy(renderer, arkaPlan_Dokusu, NULL, NULL); // arka planı çiz
            menu_ekrani_ciz(&personalBest);


        }
        if(anlik_durum == DURUM_AYARLAR)
        {
            SDL_RenderCopy(renderer, arkaPlan_Dokusu, NULL, NULL); // arka planı çiz
            ayarlar_menusu_ciz();
        }
        if(anlik_durum == DURUM_OYUNDA)
        {
            SDL_RenderCopy(renderer, arkaPlan_Dokusu, NULL, NULL); // arka planı çiz

            gemi_kontrol(&uzaygemisi, tuslar); // klavyenin anlık durumunu kontrol etden ve haraketleri yöneten fonksiyon 
            gemi_hareket_et(&uzaygemisi); // bu fonksiyon bize geminin yeni konumunu güncelleyecek
            mermileri_guncelle(mermiler); // mermilerin konumunu güncellemek için fonksyonu çağırırız

            //--------rastgele asteroit üretme işlemi ve çarpışma kontrol işlemleri -----------
            if(rand() % 70 == 0)
            {
                asteroit_uret(asteroitler);
            }
            if(rand() % 700 == 0  && puan > 10) // 1000 de bir şansla ve puan 10 dan fazla ise supply üret fonksiyonunu çalıştır
            {
                supply_uret(supplies);
            }
            supply_toplama_kontrol(supplies, &uzaygemisi, &can, &kalkan_kontrol, &kalkan_sayaci);
            asteroit_guncelle(asteroitler); // asteroitlerin konumunu güncellemek için fonksyonu çağırırız
            int hasar = asteroit_carpisma_kontrol(asteroitler, &uzaygemisi , &can, &kalkan_kontrol, &kalkan_sayaci);
            if(hasar != 0) 
            {
                Mix_PlayChannel(-1, hasar_efekti, 0);
                char hasarMetni[5];
                sprintf(hasarMetni, "%d", hasar);
                ekrana_yazi_yaz(hasarMetni,uzaygemisi.sekil.x,uzaygemisi.sekil.y,puan_font,kirmizi);
            }
            if(can <= 0)
            {
                anlik_durum = DURUM_GAMEOVER; // eğer canın 0 dan küçük veya eşitse oyunu bitir
            }
            
            if(asteroit_carpisma_kontrol_mermi(asteroitler , mermiler) == 1)
            {
                puan++;
            }
            // güncelle kontrol et çiz şeklinde olmalı yoksa çok fazla lag oluyor astroid kısmında yaşadım bu problemi 
            
            // oyun durumu oyundaysa ekrana çiz
            puan_yazdir(puan);
            can_bar_ciz(can);
            char canMetni[5];
            sprintf(canMetni, "%d", can);
            ekrana_yazi_yaz(canMetni,(EKRAN_GENISLIK/2)+400,20,puan_font,beyaz);
            gemi_ciz(renderer, &uzaygemisi , kalkan_kontrol); // geminin son kordinatlarını ekrana çizer sadece yansıtmak kalır
            mermileri_ciz(renderer, mermiler); // mermilerin son kordinatlarını ekrana çizer sadece yansıtmak kalır
            asteroit_ciz(renderer, asteroitler); // asteroitlerin son kordinatlarını ekrana çizer sadece yansıtmak kalır
            supply_ciz(renderer, supplies);
        }
        if(anlik_durum == DURUM_GAMEOVER)
        {
            SDL_RenderCopy(renderer, arkaPlan_Dokusu, NULL, NULL); // arka planı çiz
            game_over(puan , &personalBest); // ekrana game over ekler
        }

        //çizilen her şeyi ekrana yansıt
        SDL_RenderPresent(renderer);
        SDL_Delay(9);
    }
    
    // ses ile alakalı  şeyleri temizleme
    Mix_FreeChunk(ates_efekti);
    Mix_FreeChunk(patlama_efekti);
    Mix_FreeChunk(respawn_efekti);
    Mix_FreeChunk(click_efekti);
    Mix_FreeChunk(hasar_efekti);
    Mix_FreeMusic(arkaPlanMuzigi);
    Mix_CloseAudio();   

    // dokuları temizleme
    SDL_DestroyTexture(gemi_Dokusu);
    SDL_DestroyTexture(asteroit_Dokusu1);
    SDL_DestroyTexture(asteroit_Dokusu2);
    SDL_DestroyTexture(asteroit_Dokusu3);
    SDL_DestroyTexture(mermi_Dokusu);
    SDL_DestroyTexture(arkaPlan_Dokusu);
    SDL_DestroyTexture(repair_supp_Dokusu);
    SDL_DestroyTexture(shield_supp_Dokusu);
    IMG_Quit();

    // fontları temizleme
    TTF_CloseFont(puan_font);
    TTF_CloseFont(game_over_font);
    TTF_CloseFont(menu_ekrani_font); // Bunu kapatmayı da unutmuşsun, ekledim.
    TTF_Quit();

    // window ve rendererı temizleme
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
    //image kütüphanesi aktif edildi
    if (IMG_Init(IMG_INIT_PNG) == -1)
    {
        printf("Image hatasi: %s\n", IMG_GetError());
    }
    
    // font değeri atandı 
    puan_font = TTF_OpenFont("font2.ttf", 24); // puan yazdırmak için font
    game_over_font = TTF_OpenFont("font2.ttf", 60); // game over fontu için büyük bir font tanımladık 
    menu_ekrani_font = TTF_OpenFont("font2.ttf",60); // menü ekranı için büyük bir font
    if (!puan_font)
    {
        printf("Font yuklenemedi: %s\n", TTF_GetError());
    }
    if (!game_over_font)
    {
        printf("Font yuklenemedi: %s\n", TTF_GetError());
    }

    // arkaplan music değerleri atandı ve başlatıldı
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    arkaPlanMuzigi = Mix_LoadMUS("C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\arkaplan_music2.mp3");
    Mix_VolumeMusic(1); // müzik seviyesi 128 üzerinden 32 ye ayarlandı
    Mix_PlayMusic(arkaPlanMuzigi, -1); //müzik çalmaya başlandı ve sonsuz döngüye atandı -1 değikeni sonsuz döngüye sokuldu
    Mix_AllocateChannels(32);// bu fonksiyon ses kanalı sayısını 8 den 32 ye yükseltir üst üste ses genk geldiğinde tek birini oynatıyordu ondan ekledim 

    //sese efektleri belleğe yüklendi
    ates_efekti = Mix_LoadWAV("C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\laser_sound.ogg");
    patlama_efekti = Mix_LoadWAV("C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\astroidexplosive.wav");
    respawn_efekti = Mix_LoadWAV("C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\respawn.wav");
    click_efekti = Mix_LoadWAV("C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\click.ogg");
    hasar_efekti = Mix_LoadWAV("C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\damage.ogg");
    repair_efekti = Mix_LoadWAV("C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\repair.ogg");

    int baslangic_ses = 64;
    // ses efekti ses düzeyi ayarları 0 128 arası
    Mix_VolumeChunk(ates_efekti, baslangic_ses);  
    Mix_VolumeChunk(patlama_efekti, baslangic_ses-50); // patlama efektinin sesi diğerlerine göre biraz daha düşük olsun diye 20 puan azalttım
    Mix_VolumeChunk(respawn_efekti, baslangic_ses);
    Mix_VolumeChunk(click_efekti, baslangic_ses);
    Mix_VolumeChunk(hasar_efekti, baslangic_ses);
    Mix_VolumeChunk(repair_efekti, baslangic_ses);

    // pencere pointerının içini dolduruyoruz
    window = SDL_CreateWindow("Asteroids - Uzay Macerasi", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          EKRAN_GENISLIK, EKRAN_YUKSEKLIK,SDL_WINDOW_FULLSCREEN_DESKTOP);
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

    //mermi dokusu 
    mermi_Dokusu = IMG_LoadTexture(renderer, "C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\bullet.png");

    //arkaplan dokusu 
    arkaPlan_Dokusu = IMG_LoadTexture(renderer, "C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\uzay1.jpg");

    //supplies dokuları
    repair_supp_Dokusu = IMG_LoadTexture(renderer, "C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\repair_supp.png");
    shield_supp_Dokusu = IMG_LoadTexture(renderer, "C:\\Users\\pc\\Projects\\SDL2_Programlama2\\src\\shield.png");
    
    int buton_genislik = 400;  // burada standart bir buton genişliği belirledik
    int buton_yukseklik = 80;

    //baslatma butonu
    basla_butonu.sekil.x = EKRAN_GENISLIK/2 - buton_genislik/2; // butonun x kordinatını ekranın ortasına gelecek şekilde ayarlıyoruz
    basla_butonu.sekil.y = 600;
    basla_butonu.sekil.w = buton_genislik;
    basla_butonu.sekil.h = buton_yukseklik;
    basla_butonu.metin = "BASLAT";

    //ayarlar butonu 
    ayarlar_butonu.sekil.x = EKRAN_GENISLIK/2 - buton_genislik/2; // butonun x kordinatını ekranın ortasına gelecek şekilde ayarlıyoruz
    ayarlar_butonu.sekil.y = 700;
    ayarlar_butonu.sekil.w = buton_genislik;
    ayarlar_butonu.sekil.h = buton_yukseklik;
    ayarlar_butonu.metin = "AYARLAR";

    //çıkış butonu
    cikis_butonu.sekil.x = EKRAN_GENISLIK/2 - buton_genislik/2; // butonun x kordinatını ekranın ortasına gelecek şekilde ayarlıyoruz
    cikis_butonu.sekil.y = 800;
    cikis_butonu.sekil.w = buton_genislik;
    cikis_butonu.sekil.h = buton_yukseklik;
    cikis_butonu.metin = "CIKIS";

    //tekrar oyna butonu 
    tekrar_oyna_butonu.sekil.x = EKRAN_GENISLIK/2 - buton_genislik/2; // butonun x kordinatını ekranın ortasına gelecek şekilde ayarlıyoruz
    tekrar_oyna_butonu.sekil.y = 700;
    tekrar_oyna_butonu.sekil.w = buton_genislik;
    tekrar_oyna_butonu.sekil.h = buton_yukseklik;
    tekrar_oyna_butonu.metin = "TEKRAR OYNA";

    //menü butonu
    menu_butonu.sekil.x = EKRAN_GENISLIK/2 - buton_genislik/2; // butonun x kordinatını ekranın ortasına gelecek şekilde ayarlıyoruz
    menu_butonu.sekil.y = 600;
    menu_butonu.sekil.w = buton_genislik;
    menu_butonu.sekil.h = buton_yukseklik;
    menu_butonu.metin = "MENUYE DON";

    //geri dön butonu
    geri_don_butonu.sekil.x = EKRAN_GENISLIK - buton_genislik+50; // butonun x kordinatını ekranın ortasına gelecek şekilde ayarlıyoruz
    geri_don_butonu.sekil.y = 100;
    geri_don_butonu.sekil.w = buton_genislik-150;
    geri_don_butonu.sekil.h = buton_yukseklik-40;
    geri_don_butonu.metin = "GERI DON";
    
    //ses arttir butonu
    ses_arttir_butonu.sekil.x = EKRAN_GENISLIK/2 - buton_genislik - 75; // butonun x kordinatını ekranın ortasına gelecek şekilde ayarlıyoruz
    ses_arttir_butonu.sekil.y = 400;
    ses_arttir_butonu.sekil.w = buton_genislik ;
    ses_arttir_butonu.sekil.h = buton_yukseklik - 25;
    ses_arttir_butonu.metin = "SES ARTTIR";

    //ses azalt butonu
    ses_azalt_butonu.sekil.x = EKRAN_GENISLIK/2 + 100; // butonun x kordinatını ekranın ortasına gelecek şekilde ayarlıyoruz
    ses_azalt_butonu.sekil.y = 400;
    ses_azalt_butonu.sekil.w = buton_genislik - 100;
    ses_azalt_butonu.sekil.h = buton_yukseklik - 25;
    ses_azalt_butonu.metin = "SES AZALT";


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
void game_over(int puan, int *personalBest)
{
    yaziyi_ortala_ciz("GAME OVER", (EKRAN_YUKSEKLIK/2)-50, game_over_font, kirmizi);

    buton_yazdir(renderer, cikis_butonu, puan_font);
    buton_yazdir(renderer, tekrar_oyna_butonu, puan_font);
    buton_yazdir(renderer, menu_butonu, puan_font);
    
    char skorMetni[50];
    sprintf(skorMetni, "Skor: %d", puan);
    yaziyi_ortala_ciz(skorMetni,(EKRAN_YUKSEKLIK/2)-100,puan_font,beyaz);
    if(puan > *personalBest)
    {
        *personalBest = puan;
    }
}
void menu_ekrani_ciz(int *personalBest)
{
    yaziyi_ortala_ciz("ASTEROID OYUNU",(EKRAN_YUKSEKLIK/2)-150,game_over_font,kirmizi);

    char bestSkorMetni[50];
    sprintf(bestSkorMetni, "EN IYI SKOR = %d", *personalBest); 
    yaziyi_ortala_ciz(bestSkorMetni,100,puan_font,beyaz);

    buton_yazdir(renderer, basla_butonu, puan_font);
    buton_yazdir(renderer, ayarlar_butonu, puan_font);
    buton_yazdir(renderer, cikis_butonu, puan_font);


}
void ekrana_yazi_yaz(const char *metin, int x, int y, TTF_Font *secilen_font, SDL_Color renk)
{
    //yüzey oluştur ve onu dokuya çevir
    SDL_Surface *yuzey = TTF_RenderText_Solid(secilen_font, metin, renk);
    SDL_Texture *doku = SDL_CreateTextureFromSurface(renderer, yuzey);
    
    //atama yapmak için bir kutu ayarla
    SDL_Rect kutu = {x, y, yuzey->w, yuzey->h};
    
    //dokuyu kutuya yapıştır
    SDL_RenderCopy(renderer, doku, NULL, &kutu);
    
    // çöpleri temizle
    SDL_FreeSurface(yuzey);
    SDL_DestroyTexture(doku);
}
void yaziyi_ortala_ciz(char *metin, int y_kordinati, TTF_Font *font, SDL_Color renk) 
{
    int genislik, yukseklik;
    //yazının genişlini ve yüksekliğini hesapla
    TTF_SizeUTF8(font, metin, &genislik, &yukseklik);
    
    // ekrana yazının ortalanmış şekilde çizilebilmesi için x kordinatını hesapla
    int x_kordinati = (EKRAN_GENISLIK / 2) - (genislik / 2);
    
    //bunları ekrana yazdır
    ekrana_yazi_yaz(metin, x_kordinati, y_kordinati, font, renk);
}
void can_bar_ciz(int can)
{
    SDL_Rect canBar;
    SDL_Rect canBarArkaPlan;
    canBar.x = EKRAN_GENISLIK-500;
    canBar.y = 20;
    canBar.w = can*4;
    canBar.h = 35;
    canBarArkaPlan.x = EKRAN_GENISLIK-500;
    canBarArkaPlan.y = 20;
    canBarArkaPlan.w = 400;
    canBarArkaPlan.h = 35;
    // can barının boyutunu can değerine göre ayarlıyoruz
    SDL_SetRenderDrawColor(renderer, 10, 30, 100, 0);
    SDL_RenderFillRect(renderer, &canBarArkaPlan); // can barının arka planını ekrana çiziyoruz
    SDL_SetRenderDrawColor(renderer, 17, 138, 21, 255); // can barının rengini yeşil yapıyoruz
    SDL_RenderFillRect(renderer, &canBar); // can barını ekrana çiziyoruz
}
int buton_tiklama_kontrol(int fare_x, int fare_y, Buton buton)
{
    if(fare_x >= buton.sekil.x && fare_x <= buton.sekil.x + buton.sekil.w && fare_y >= buton.sekil.y && fare_y <= buton.sekil.y + buton.sekil.h)
    {
        return 1; // mouse kutunun içindeyse 1 döndür
    }
    return 0; // değilse 0 döndür
}
void buton_yazdir(SDL_Renderer *renderer,Buton buton, TTF_Font *font)
{
    int fare_x;
    int fare_y;
    SDL_GetMouseState(&fare_x, &fare_y); // fare kordinatlarını alıyoruz ve değişkenlere atıyoruz

    if(buton_tiklama_kontrol(fare_x, fare_y, buton)) // eğer fare butonun içindeyse butonun rengini değiştir
    {
        SDL_SetRenderDrawColor(renderer, 150, 150, 200, 255); 
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 100, 100, 150, 0);
    }
    SDL_RenderFillRect(renderer, &buton.sekil); //burada da o renk ile çiziyoruz

    int yazi_genislik;
    int yazi_yukseklik;
    TTF_SizeUTF8(font, buton.metin, &yazi_genislik, &yazi_yukseklik);  // burada yazının genişliğini bulmak için değişkenleri adres olarak yolluyoruz

    int yazi_x = buton.sekil.x + (buton.sekil.w / 2) - (yazi_genislik / 2);   // buradada bulduğumuz uzunlukları kutuunun ortasını bulmak için kullanıyoruz
    int yazi_y = buton.sekil.y + (buton.sekil.h / 2) - (yazi_yukseklik / 2);

    ekrana_yazi_yaz(buton.metin, yazi_x, yazi_y, font, beyaz); // yazıyı kutunun ortasına yazdırıyoruz
}
void ayarlar_menusu_ciz()
{
    yaziyi_ortala_ciz("AYARLAR",130,game_over_font,kirmizi);
    
    int anlik_ses = Mix_VolumeMusic(-1); // güncel ses seviyesini alır
    int yuzdelik_ses = (anlik_ses * 100) / 128;  // bu değeri 0 100 arasına dönüştürür
    
    char sesMetni[50];
    sprintf(sesMetni, "Ses Seviyesi: %d", yuzdelik_ses);
    ekrana_yazi_yaz(sesMetni, (EKRAN_GENISLIK/2)-100, 300, puan_font, beyaz);
    
    buton_yazdir(renderer, geri_don_butonu, puan_font);
    buton_yazdir(renderer, ses_arttir_butonu, puan_font);
    buton_yazdir(renderer, ses_azalt_butonu, puan_font);
}

