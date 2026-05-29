# 🚀 Asteroids: Uzay Macerası

Bu proje, **Kocaeli Üniversitesi Programlama 2** dersi dönem projesi olarak geliştirilmiş, **C dili** ve **SDL2** kullanılarak yazılmış bir uzay aksiyon oyunudur.

---

# ✨ Oyun Özellikleri

## ☄️ 1. Asteroids Mekanikleri

Ekranın bir sınırından çıkıldığında diğer tarafından tekrar giriş yapılabilir.
Uzay gemisi ve asteroitler için kesintisiz bir oyun döngüsü sunar.

---

## 🛰️ 2. Akıcı Hareket ve Fizik Sistemi

Trigonometrik fonksiyonlar kullanılarak hesaplanan:

* İvmelenme sistemi
* Gemi sürtünmesi
* 360 derece dönebilen hassas kontrol mekanizması

ile akıcı ve gerçekçi bir uzay hareket deneyimi sağlanır.

---

## 🎮 3. Çoklu Oyun Modları

Farklı oyun tarzlarına uygun çeşitli modlar bulunmaktadır:

* ⏱️ **1 Dakika Modu**
* ⏱️ **5 Dakika Modu**
* ♾️ **Sonsuz Mod**

---

## 💥 4. Dinamik Asteroit Parçalanması

Mermi ile vurulan büyük ve orta boy asteroitler anında yok olmaz.

Bunun yerine:

* Daha küçük parçalara ayrılırlar
* Farklı yönlere doğru dağılırlar
* Oyunun temposunu ve zorluğunu artırırlar

---

## 🎨 5. Özelleştirilebilir Görünüm

Ayarlar menüsü üzerinden oyuncular:

* 3 farklı uzay gemisi tasarımı seçebilir
* Uzay arka planını değiştirebilir

---

## 🛡️ 6. Taktiksel Güçlendirmeler

Oyun sırasında rastgele ortaya çıkan güçlendirmeler:

### ❤️ Tamir Kiti

Geminin canını **20 puan** yeniler.

### 🛡️ Enerji Kalkanı

3 saniye boyunca aktif kalan geçici bir koruma sağlar.
Kalkan aktifken asteroit çarpmalarından hasar alınmaz.

---

## 📊 7. Kapsamlı Arayüz ve Skor Takibi

Oyunda aşağıdaki arayüz sistemleri bulunmaktadır:

* Ana Menü
* Oyun İçi Can Barı
* Süre Göstergesi
* Oyun Sonu Ekranı
* Çalışma süresi boyunca kayıtlı **En İyi Skor** sistemi

---

# 🎮 Oynanış

## ⌨️ Klavye ve Fare Kontrolleri

| Tuş               | İşlev                                      |
| ----------------- | ------------------------------------------ |
| **W**             | Gemiyi ileri doğru ivmelendirir            |
| **S**             | Gemiyi yavaşlatır / geriye hareket ettirir |
| **A / D**         | Gemiyi sola veya sağa döndürür             |
| **SPACE**         | Lazer silahını ateşler                     |
| **ESC**           | Ayarlar menüsünü açar / oyunu duraklatır   |

---

# 🕹️ Nasıl Oynanır?

## ☄️ Parçalanan Asteroitler

Asteroitleri vurarak puan kazanabilirsin.

Ancak dikkatli ol:

* Büyük ve orta boy asteroitler yok olmaz
* Daha küçük ve hızlı iki parçaya ayrılırlar

---

## 🌌 Sonsuz Uzay Mekaniği

Ekranın:

* Sağından çıkarsan solundan
* Üstünden çıkarsan altından tekrar oyuna giriş yaparsın.

Bu mekanik sayesinde sıkıştığın anlarda tehlikeden kaçabilirsin.

---

# 🛠️ Kurulum ve Derleme

Bu proje, derleme süreçlerinin otomatikleştirilmesi ve platformlar arası taşınabilirlik için **CMake** yapılandırma sistemini kullanmaktadır.

---

## 📌 1. Gereksinimler

Projeyi derlemek için aşağıdaki araçların kurulu olması gerekir:

* **CMake**
* **GCC / MinGW Derleyicisi**

  * Windows için **MSYS2 UCRT64** önerilir
* SDL2 geliştirme kütüphaneleri:

  * `SDL2`
  * `SDL2_image`
  * `SDL2_mixer`
  * `SDL2_ttf`

---

## 📦 2. Kütüphanelerin Kurulumu (Windows / MSYS2)

MSYS2 UCRT64 terminalinde aşağıdaki komutu çalıştırın:

```bash
pacman -S mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_image mingw-w64-ucrt-x86_64-SDL2_mixer mingw-w64-ucrt-x86_64-SDL2_ttf
```

---

## 📥 3. Projeyi Klonlama

```bash
git clone https://github.com/kullanici_adi/asteroids-uzay-macerasi.git
cd asteroids-uzay-macerasi
```

---

# ⚙️ 4. Yöntem A: VS Code ile Derleme ve Çalıştırma

## ✅ En Kolay Yol

1. VS Code içerisine **CMake Tools** eklentisini kurun.
2. `Ctrl + Shift + P` ile Komut Paleti’ni açın.
3. `CMake: Select a Kit` komutunu çalıştırın.
4. Derleyicinizi seçin
5. Alt durum çubuğundaki:

   * **Build** butonu ile derleyin
   * **Play** butonu ile çalıştırın

---

# 🧱 5. Yöntem B: Terminal Üzerinden Derleme

Projeyi manuel olarak derlemek için:

```bash
# 1. Build klasörü oluştur
mkdir build
cd build

# 2. CMake yapılandırmasını başlat
cmake -G "MinGW Makefiles" ..

# 3. Projeyi derle
cmake --build .
```

Derleme tamamlandıktan sonra oluşan:

```bash
./AsteroidsUzayMacerasi
```

komutu ile oyunu çalıştırabilirsiniz.

---

# 🌠 Proje Hakkında

Bu proje:

* SDL2 ile oyun geliştirme,
* Gerçek zamanlı fizik hesaplamaları,
* Oyun döngüsü yönetimi,
* UI sistemleri,
* CMake tabanlı proje organizasyonu konularında deneyim kazanmak amacıyla geliştirilmiştir.

# Kaynakça 

Bu projede:

* Ses dosyaları = https://kenney.nl/
* Görseller = https://pngtree.com/
* Font = Russe one 