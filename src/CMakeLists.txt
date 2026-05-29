cmake_minimum_required(VERSION 3.10)

# Proje adını Space_War yaptık, böylece ${PROJECT_NAME} yazan her yer otomatik güncellenecek
project(Space_War C)

# C Standart ayarı (Trigonometrik fonksiyonlar ve standart yapılar için C99 idealdir)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 1. Projedeki tüm kaynak (.c) dosyalarını tanımlayalım
set(SOURCES
    main.c
    gemi.c
    asteroit.c
    mermi.c
    supplies.c
)

# 2. SDL2 ve gerekli tüm eklenti kütüphanelerini sistemde ara
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# 3. .exe dosyasının çıkacağı klasör: 
# DLL'leri ve resimleri (assets) kolayca bulması için aktif edildi.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src")

# 4. Derlenecek hedef çalıştırılabilir (.exe) dosyasını tanımla
add_executable(${PROJECT_NAME} ${SOURCES})

# 5. Derleyiciye başlık (.h) dosyalarının nerede olduğunu bildir
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# 6. Kütüphane dosyalarını (.lib / .a) projeye bağla (Linkleme işlemi)
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)