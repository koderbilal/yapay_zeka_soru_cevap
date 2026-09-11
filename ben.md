# giriş 

# 1. soru 

 merhaba c++ ile bir oyun yapmak istiyorum.

bu yapacağım oyun bir sandbox olucak.

yapacağım oyunda envanter sistemi olucak.

yapacağım oyunda açık barı olucak.

bu bahsettiğim oyunda can barı olucak.

bu oyunda bosslar, yaratıklar ve canlılar olucak.

yapacağım oyunda minecraft oynundaki gibi kare kare tasarımı benimseyeceğim, yapacağım oyunu büyük çoğunlukla minecrafttaki blok adları ve item adları olucak çünkü minecraft bedrock editionda benim tekture packi ve o tekturem ile ilgili assests lerim va ve ben bunları kullanmak istiyorum 

# 1. cevap

Kendi Minecraft benzeri (voxel tabanlı) sandbox oyununu C++ ile geliştirmek teknik açıdan oldukça zorlu ama bir o kadar da mükemmel bir mühendislik projesi. Halihazırda elinde Bedrock Edition için hazırladığın doku paketlerinin (texture pack) ve asset'lerin olması, seni görsel tasarım yükünden kurtarıp doğrudan kodlamaya ve mekaniklere odaklanmanı sağlayacak çok büyük bir avantaj.

C++ ile bu tür bir projeye başlarken en kritik karar, temel altyapıyı nasıl kuracağını seçmektir. Özellikle oyunu ileride farklı platformlara (örneğin Android cihazlara) taşımayı düşünüyorsan, projeyi ve derleme sistemini (örn. CMake) baştan platform bağımsız kurgulamak işini çok kolaylaştıracaktır.

## Altyapı Seçimi: Motor mu, Sıfırdan mı?

Önünde iki ana yol bulunuyor:

| Yaklaşım | Avantajları | Dezavantajları | Önerilen Araçlar |
| --- | --- | --- | --- |
| **Oyun Motoru (Örn: Godot + C++)** | Arayüz (can/açlık barı), fizik motoru ve mobil platformlara (Android) çıktı almak hazır gelir. GDExtension ile çekirdek mekanikler tamamen C++ ile yazılır. | Voxel mimarisi oyun motorları için doğal bir yapı değildir. Optimizasyon için motorun render sistemini modifiye etmen gerekebilir. | Godot Engine, C++ |
| **Özel Motor (Sıfırdan C++)** | Voxel mimarisinde (Chunk'lar) maksimum performans ve bellek üzerinde tam kontrol sağlar. Sadece ihtiyacın olan kod çalışır. | Çarpışma fizikleri, UI çizimleri (barlar) ve platformlar arası derleme işlemlerini senin halletmen gerekir. | OpenGL/Vulkan, SDL2 (Pencere/Girdi), GLM (Matematik) |

Zamanını en verimli şekilde kullanmak istiyorsan, **Godot Engine ve C++ (GDExtension)** kombinasyonu; envanter sistemi, can/açlık barları ve arayüz (UI) tasarımı gibi konularda seni aylarca sürecek bir iş yükünden kurtarır. Sadece dünyayı oluşturan blokların mantığını C++ tarafında yazarsın.

---

## İstediğin Sistemlerin C++ Mimarisindeki Karşılıkları

Bahsettiğin özellikleri kurgularken kullanman gereken temel yazılım mimarileri şunlardır:

* **Dünya Üretimi (Chunk Sistemi):** Minecraft tarzı oyunlarda tüm dünyayı aynı anda yüklemek sistemi kilitler. Dünyayı 16x16x256 blokluk "Chunk"lara (yığınlara) bölmeli ve sadece oyuncunun görüş alanındaki Chunk'ları render etmelisin.
* **Asset ve Texture Entegrasyonu:** Bedrock doku paketlerini okumak için C++ tarafında `stb_image` (PNG dosyalarını okumak için) ve `nlohmann/json` (blok verilerini ve modelleri okumak için) gibi hafif kütüphaneler kullanabilirsin. Blok adlarını `std::unordered_map` yapısında tutarak, Minecraft isimlerini kendi oyun içi ID'lerinle eşleştirebilirsin (Örn: `"minecraft:dirt" -> 1`).
* **Varlıklar, Yaratıklar ve Boss'lar (ECS):** Boss'lar ve diğer canlılar için Entity Component System (ECS) mimarisi kullanman en doğrusudur. Böylece "Can Barı" veya "Hareket Etme" yeteneğini bir bileşen (component) olarak tanımlayıp, bunu hem oyuncuya, hem zombiye, hem de boss'a tek satır kodla ekleyebilirsin.
* **Arayüz (Can ve Açlık Barları):** 3D dünyanın üzerine 2D bir matris katmanı (Orthographic Projection) çizdirerek arayüzleri oluşturursun. Karakterinin can veya açlık değişkeni (örn: `int currentHunger`) azaldığında, ekrana çizdirilen ikonların (veya barın) genişliğini matematiksel olarak kırparsın.

---

## Geliştirme Adımları

Projeye başlarken her şeyi aynı anda yapmak yerine şu sıralamayı takip etmen, motivasyonunu yüksek tutacaktır:

1. **Pencere ve Kamera:** Serbestçe dolaşabilen (uçan) bir 3D kamera oluştur.
2. **İlk Blok ve Doku:** Bedrock texture pack'inden bir toprak (dirt) dokusu çekip ekrana tek bir küp çizdir.
3. **Chunk Optimizasyonu:** O küpü binlerce blokluk bir yığına çevir. En önemli optimizasyon kuralı: *Sadece blokların dışarıdan görünen yüzeylerini çizdir (Greedy Meshing veya Face Culling).*
4. **Çarpışma (Collision):** Oyuncunun blokların içinden geçmesini engelle ve yerçekimini ekle.
5. **Arayüz:** Envanteri açma/kapama fonksiyonunu, can ve açlık barlarını ekrana yerleştir.



# 2. soru 

evet geçelim, ben özel motor tercih edeceğim, kendime ait bir motorum olsun istiyorum bu sayede ileride modlama açısından daha fazla özellik hakkına sahip olacağını düşünüyorum, daha fazla özellik ve daha fazla içerik getirmek için bir zorunluluk olduğunu düşünüyorum.

# 2. cevap

Özel bir motor (custom engine) geliştirmeyi seçmen, özellikle voxel tabanlı bir oyun ve geniş modlama desteği (modding API) planlıyorsan en vizyoner adımdır. Başkasının motoruna bağımlı kalmadan, oyunun belleği (RAM) nasıl kullanacağına ve blokların nasıl güncelleneceğine tamamen sen karar verirsin. İleride oyunu mobil cihazlara veya farklı ortamlara taşımak istediğinde çekirdek yapıyı esnetebilmek bu sayede çok daha kolay olacaktır.

Sıfırdan bir motor yazarken tekerleği yeniden icat etmemek için endüstri standardı olan çapraz platform kütüphaneleri kullanmalıyız.

* **Pencere ve Girdi:** `SDL2` veya `GLFW` (SDL2, çoklu platform ve mobil derleme süreçlerinde çok daha esnektir).
* **Grafik API:** `OpenGL 3.3 Core` (İleride mobil platformlara geçerken OpenGL ES'e uyarlaması kolaydır).
* **Matematik işlemleri:** `GLM` (Vektör ve matris hesaplamaları için).
* **Doku (Texture) Yükleme:** `stb_image.h` (Bedrock PNG'lerini okumak için tek dosyalık harika bir kütüphane).

İşte ekrana ilk bloğunu çizdirmek ve kendi dokunu ona giydirmek için kurman gereken temel mimari:

### 1. Küpün Verilerini (Vertex & UV) Tanımlamak

OpenGL'de 3D bir küp çizdirmek için küpün köşelerini (Vertex) ve o köşelere dokunun (Texture) nasıl sarılacağını belirten UV koordinatlarını tanımlaman gerekir. Bir küp 6 yüzeyden, her yüzey 2 üçgenden oluşur.

```cpp
// X, Y, Z (Pozisyon) ve U, V (Doku Koordinatları)
float vertices[] = {
    // Ön Yüzey
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Sol Alt
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Sağ Alt
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Sağ Üst
     ...
     // Bu şekilde 36 adet nokta (vertex) tanımlanır.
};

```

Bu verileri ekran kartına göndermek için **VBO** (Vertex Buffer Object) ve **VAO** (Vertex Array Object) kullanmalısın.

### 2. Bedrock Dokusunu Yüklemek ve "Kare Kare" Görüntüyü Korumak

Minecraft tarzı oyunların en kritik görsel sırrı "Texture Filtering" (Doku Filtreleme) ayarıdır. Bedrock paketindeki dokular genellikle 16x16 pikseldir. Varsayılan ayarlarda OpenGL bu küçük görselleri büyütürken bulanıklaştırır (blur).

Bulanıklığı engellemek ve o keskin, pikselli Minecraft görünümünü elde etmek için `GL_NEAREST` filtresini kullanmalısın:

```cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);

// Pikselli görünümü sağlayan o kritik ayar:
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

int width, height, nrChannels;
// Bedrock pack içindeki dirt.png dosyasını yükle
unsigned char *data = stbi_load("assets/textures/blocks/dirt.png", &width, &height, &nrChannels, 0);

if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
stbi_image_free(data);

```

### 3. Shader (Gölgelendirici) Programlarını Yazmak

C++ kodu doğrudan ekrana çizim yapmaz; bu işi ekran kartındaki Shader'lar yapar. GLSL diliyle yazılmış iki basit dosyaya ihtiyacın olacak:

**Vertex Shader (`vertex.glsl`):** 3D uzaydaki noktanın ekrandaki yerini hesaplar.

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

```

**Fragment Shader (`fragment.glsl`):** Yüklediğin dokunun piksellerini küpün üzerine yapıştırır.

```glsl
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;

void main() {
    FragColor = texture(ourTexture, TexCoord);
}

```

### 4. Render Döngüsü (Oyun Motorunun Kalbi)

Motorunun `while(oyunCalisiyor)` şeklindeki ana döngüsünde (Game Loop) ekranı temizleyip bu küpü sürekli yeniden çizdirmelisin.

```cpp
while (!quit) {
    // 1. Ekranı temizle (Gökyüzü rengi)
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. Texture ve Shader'ı aktif et
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.use();

    // 3. Küpü çiz
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 4. Ekrana yansıt (Double Buffering)
    SDL_GL_SwapWindow(window);
}

```

Ekranda hareketsiz, dümdüz duran topraktan bir blok elde ettiğinde oyun motorunun en zorlu ilk eşiğini atlamış olacaksın.

# 3. soru

evet birde ben bu kodları nereye yazacağım 

# 3. cevap 

Bu harika bir soru. Özel bir motor geliştirdiğin için her şeyin temelini senin atman gerekiyor. Kodları yazacağın ortamı (IDE) ve dosyaların klasör yapısını baştan doğru kurmazsak, proje ilerledikçe kodlar içinden çıkılmaz bir spagettiye dönüşür.

İleride bu motoru Android'e taşıma ve dışarıdan mod/texture pack yükleme hedeflerini göz önünde bulundurarak, sana endüstri standardı olan çapraz platform (cross-platform) yapısını kuralım.

## 1. Kodları Hangi Programa Yazacaksın? (IDE Seçimi)

Özel bir C++ oyun motoru yazarken ve ileride bunu mobil platformlara derleme hedefin varken en iyi seçeneğin **Visual Studio Code (VS Code)** ve **CMake** ikilisidir.

* **Neden VS Code?** Çok hafiftir, kendi terminali içindedir ve C++ eklentileriyle devasa bir IDE'ye dönüşür.
* **Neden CMake?** Kodlarını hem Windows'ta (.exe), hem Linux'ta, hem de Android'de (.apk / .so) çalışacak şekilde derlemeni sağlayan evrensel bir kurallar bütünüdür. Kendi oyun motorunu yazan herkesin yolu CMake'ten geçer.

## 2. Motorun Klasör Yapısı Nasıl Olmalı?

Tüm kodları tek bir dosyaya yazmayacağız. Proje klasörünü bilgisayarında şu düzende oluşturmalısın:

```text
BenimMotorum/
├── CMakeLists.txt        # Projenin derleme kuralları (Hangi kütüphaneler eklenecek)
├── src/                  # C++ kaynak kodlarının (.cpp) durduğu yer
│   ├── main.cpp          # Pencerenin açıldığı ve Oyun Döngüsü'nün çalıştığı ana dosya
│   ├── Camera.cpp        # Kameranın hareket kodları
│   └── Shader.cpp        # Ekran kartına gönderilen grafik kodları
├── include/              # Başlık dosyalarının (.h) durduğu yer
│   ├── Camera.h          # Kameranın özellikleri (Canı, hızı, pozisyonu)
│   └── Shader.h
└── assets/               # Oyunun dosyaları (Bedrock pack'ini buraya koyacaksın)
    ├── textures/         # Örn: dirt.png, stone.png
    └── shaders/          
        ├── vertex.glsl   # 3D hesaplama gölgelendiricisi
        └── fragment.glsl # Renk/Doku gölgelendiricisi

```

Bu yapıyı kurduktan sonra `main.cpp` içine bir önceki adımda bahsettiğim ekranı temizleme ve bloğu çizdirme döngüsünü (Game Loop) koyacaksın.

---

## 3. Kamerayı Eklemek (3D Dünyada Dolaşmak)

Grafik programlamada aslında fiziksel bir "kamera" yoktur. Sen "ileri" gitmek istediğinde, aslında dünyadaki tüm blokları "geriye" doğru kaydırırsın. Bu matematiksel illüzyonu **GLM (OpenGL Mathematics)** kütüphanesindeki `glm::lookAt` fonksiyonu ile yapıyoruz.

Kamera sistemini `include/Camera.h` dosyasının içine şu şekilde tanımlamalısın:

```cpp
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // Kameranın 3 Temel Vektörü
    glm::vec3 Position; // Oyuncunun dünyadaki konumu (X, Y, Z)
    glm::vec3 Front;    // Oyuncunun baktığı yön
    glm::vec3 Up;       // Dünyanın yukarısı (Kafamızın yan yatmaması için)

    // Fare açıları
    float Yaw;          // Sağa/Sola bakma açısı
    float Pitch;        // Yukarı/Aşağı bakma açısı
    float Speed;        // Yürüme/Uçma hızı

    Camera() {
        Position = glm::vec3(0.0f, 0.0f, 3.0f); // Başlangıç noktası
        Front    = glm::vec3(0.0f, 0.0f, -1.0f); // Z ekseninde ileriye bak
        Up       = glm::vec3(0.0f, 1.0f, 0.0f);  // Y ekseni "yukarı"dır
        Yaw      = -90.0f;
        Pitch    = 0.0f;
        Speed    = 2.5f;
    }

    // Ekran kartına gönderilecek olan Görüntü Matrisini (View Matrix) hesaplar
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }
};

```

### Klavye ve Fareyi Bağlamak

Bu sınıfı oluşturduktan sonra `src/main.cpp` içindeki tuş okuma sistemine (SDL2 veya GLFW) şu mantığı eklersin:

* **WASD Kontrolü:**
* **W'ye basılırsa:** `Position += Speed * Front;` (Baktığın yöne doğru ilerle)
* **S'ye basılırsa:** `Position -= Speed * Front;` (Baktığın yönün tersine git)


* **Fare Kontrolü:** Fareyi oynattığında `Yaw` (X ekseni) ve `Pitch` (Y ekseni) açılarını güncelleyip, trigonometri (sinüs/kosinüs) kullanarak yeni bir `Front` (Bakış yönü) vektörü hesaplatırsın.

# 4. Soru

Projemdeki dosyaları, kütüphaneleri (SDL2, OpenGL, GLM) birbirine bağlayıp bu kodu derlenebilir hale getirmek için CMakeLists.txt dosyamı nasıl yazmalıyım?

# 4. cevap

CMake, aslında projenin anayasasıdır. İleride oyunun çekirdek sistemini tamamlayıp Android platformunda sürüm yayımlamak istediğinde, Android NDK'nın (Native Development Kit) da doğrudan CMake kullanarak C++ kodlarını derlediğini göreceksin. Bu yüzden C++ tabanlı projelerde derleme sürecini baştan bu standartla kurmak sana büyük bir zaman kazandıracaktır.

İşte `BenimMotorum` klasörünün ana dizinine oluşturacağın `CMakeLists.txt` dosyasının içeriği:

```cmake
# CMake'in minimum sürüm gereksinimi
cmake_minimum_required(VERSION 3.15)

# Projemizin adı ve versiyonu
project(VoxelEngine VERSION 1.0)

# Modern C++ standardını (C++17) zorunlu kılıyoruz
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Kendi yazdığımız başlık dosyalarının (.h) bulunduğu klasör
include_directories(include)

# Sistemde kurulu olan kütüphaneleri bul (SDL2 ve OpenGL)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

# Derlenecek tüm kaynak (.cpp) dosyalarımızın listesi
set(SOURCES
    src/main.cpp
    src/Camera.cpp
    src/Shader.cpp
)

# Çalıştırılabilir (.exe veya Linux/Android çalıştırılabilir dosyası) motorumuzu oluştur
add_executable(${PROJECT_NAME} ${SOURCES})

# Bulunan kütüphaneleri motorumuza bağla (Linkleme işlemi)
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    SDL2::SDL2
)

```

### Kütüphaneleri Ortama Kurmak ve Derlemek

Terminal kullanarak derleme yapmak ve sistem komutlarını yönetmek projelerde sana tam kontrol sağlar. Geliştirme sürecini terminal üzerinden, yapılandırdığın Debian 13 ortamında yürütüyorsan, CMake'in bulabilmesi için gerekli kütüphaneleri sisteme önceden kurman gerekir. Bunun için şu komutu çalıştırman yeterlidir:

```bash
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libglm-dev

```

*(Not: `stb_image.h` sadece bir başlık dosyası olduğu için onu kurmana gerek yoktur, doğrudan `include/` klasörünün içine atman yeterlidir.)*

Sistem hazır olduktan sonra, kodlarının olduğu ana dizinde terminali açıp şu klasik CMake döngüsünü çalıştırarak oyun motorunu derleyebilirsin:

```bash
# Derleme dosyalarının karışmaması için ayrı bir klasör oluştur
mkdir build
cd build

# CMake kurallarını oku ve Makefile oluştur
cmake ..

# Kodu derle
make

# Motoru çalıştır!
./VoxelEngine

```

Bu adımları tamamladığında, `main.cpp` içine yazdığın o ilk OpenGL kodları çalışacak, ekranda SDL2 ile oluşturulmuş bir pencere açılacak ve kendi kameranla etrafında dolaşabildiğin o ilk Bedrock dokulu toprak bloğunu göreceksin. Motorunun temelleri artık atılmış oldu.


# 5. soru

Sıradaki adım olan Chunk (Yığın) mimarisini kurup binlerce bloğu aynı anda oyun kasmadan ekrana nasıl çizdireceğimi anlatır mısın?

# 5. cevap

Voxel tabanlı oyunlarda oyunun kaderini belirleyen en kritik aşamaya geldik. Eğer her bir bloğu ekrana tek tek çizdirmeye kalkarsan (binlerce kez `glDrawArrays` çağrısı yaparak), en güçlü bilgisayarlar bile saniyede 5 kare (FPS) vererek çökecektir. Ekran kartları binlerce küçük komutu değil, tek bir büyük komutu sevecek şekilde tasarlanmıştır.

Bunu çözmek için **Chunk (Yığın)** mimarisini ve **Face Culling (Görünmeyen Yüzeyleri Gizleme)** tekniğini kullanacağız.

Minecraft'ta bir Chunk standart olarak **16x16 genşliğinde ve 256 (veya 320) blok yüksekliğindedir**.

---

## 1. Chunk Verisini RAM'de Saklamak

Öncelikle 16x256x16 boyutundaki bu devasa alanı RAM'de nasıl tutacağımızı belirlemeliyiz. Her bloğun bir ID'si (Kimlik numarası) olmalıdır. (Örn: 0 = Hava, 1 = Toprak, 2 = Taş).

Bunun için basit bir `Chunk` sınıfı oluşturuyoruz:

```cpp
#include <vector>

class Chunk {
public:
    // 16x256x16 = 65,536 blok. uint8_t kullanarak her blok için sadece 1 byte harcarız.
    uint8_t blocks[16][256][16]; 
    
    // Sadece dışarıdan görünen yüzeylerin noktalarını tutacağımız liste
    std::vector<float> meshVertices;
    
    unsigned int VAO, VBO;

    Chunk() {
        // Dünyayı başlangıçta hava (0) ile doldur, alt katmanları toprak (1) yap.
        for(int x = 0; x < 16; x++) {
            for(int y = 0; y < 256; y++) {
                for(int z = 0; z < 16; z++) {
                    if (y < 64) blocks[x][y][z] = 1; // 64. seviyeye kadar toprak
                    else blocks[x][y][z] = 0;        // Üstü hava
                }
            }
        }
    }
};

```

---

## 2. Optimizasyonun Sırrı: Face Culling (Sadece Görüneni Çiz)

Bir toprağın her tarafı toprakla kaplıysa, onun hiçbir yüzeyini oyuncu göremez. O halde ekran kartına bu bloğu göndermenin hiçbir anlamı yoktur.

Sadece yanında "Hava (0)" veya saydam bir blok (örneğin Cam) olan yüzeyleri hesaplayıp, `meshVertices` listemize ekleyeceğiz. Bu işleme "Mesh Generation" (Ağ Üretimi) denir.

```cpp
void GenerateMesh() {
    meshVertices.clear(); // Eski veriyi temizle

    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 256; y++) {
            for(int z = 0; z < 16; z++) {
                
                // Eğer blok havaysa çizmeye gerek yok, geç!
                if (blocks[x][y][z] == 0) continue; 

                // ÜST yüzey kontrolü (Y + 1)
                if (y == 255 || blocks[x][y+1][z] == 0) {
                    // Üst yüzeyin Vertex ve Texture(UV) koordinatlarını hesapla
                    // meshVertices.push_back(...) ile listeye ekle
                }

                // ALT yüzey kontrolü (Y - 1)
                if (y == 0 || blocks[x][y-1][z] == 0) {
                    // Alt yüzeyi listeye ekle
                }

                // SAĞ yüzey (X + 1), SOL yüzey (X - 1)
                // ÖN yüzey (Z + 1), ARKA yüzey (Z - 1)
                // Hepsini aynı mantıkla komşu bloğa bakarak kontrol et...
            }
        }
    }
}

```

> **Önemli Not:** Bu mantık sayesinde, 65.536 bloğun bulunduğu bir Chunk'ta, eğer hepsi doluysa sadece en dıştaki yüzeyleri (belki 1000-2000 üçgen) çizersin. İçeride kalan milyonlarca üçgen ekran kartına hiç gitmez. Oyunun 60+ FPS çalışmasının anahtarı tam olarak bu koşullardır.

---

## 3. Ekran Kartına Göndermek (Sıfır Kasma)

Mesh (Ağ) oluşturma işlemi bittikten sonra, elimizdeki bu binlerce noktalık tek bir listeyi ekran kartına (VBO'ya) göndeririz.

```cpp
void BufferData() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Oluşturduğumuz büyük listeyi ekran kartına kopyala
    glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(float), meshVertices.data(), GL_STATIC_DRAW);

    // Vertex ve Texture özelliklerini ayarla (glVertexAttribPointer...)
}

```

Oyun döngünün (Game Loop) içine geldiğimizde ise o harika an gerçekleşir. 65 bin bloğu çizdirmek için binlerce satır kod çalışmaz. Her Chunk için sadece şu satır çalışır:

```cpp
glBindVertexArray(chunk.VAO);
glDrawArrays(GL_TRIANGLES, 0, chunk.meshVertices.size() / 5);

```

Bir oyuncu kazmasıyla bir bloğu kırdığında (örneğin Toprak bloku Hava olduğunda), dizideki değeri `blocks[x][y][z] = 0` yaparız, `GenerateMesh()` fonksiyonunu bir kez daha çağırıp yeni yüzeyleri hesaplarız ve `BufferData()` ile ekran kartına yeni veriyi güncelleriz.

# 6. soru

Sonsuz dünya üretimini (Infinite Terrain Generation) ve Perlin Noise algoritmasını kullanarak tepe, dağ ve vadileri nasıl oluşturacağımı anlatır mısın?

# 6. cevap

Bir oyunu sadece küplerden oluşan düz bir zeminden çıkarıp, "yaşayan, keşfedilebilir bir dünyaya" dönüştüren sihir tam olarak **Perlin Noise** (veya benzeri gürültü) algoritmalarıdır.

Eğer haritayı oluştururken standart C++ `rand()` fonksiyonunu kullansaydık, her blok rastgele bir yükseklikte olurdu ve ortaya televizyon karıncalanması gibi, yürünemez bir kaos çıkardı. Perlin Noise ise **"sürekli ve yumuşak geçişli rastgelelik"** sağlar. Yani bir blok 70. yüksekteyse, yanındaki blok rastgele 10. yüksekliğe düşmez; 69, 70 veya 71 olur. Bu da tepeleri ve vadileri oluşturur.

Bu sistemi motorumuza nasıl entegre edeceğimizi adım adım inceleyelim:

## 1. Doğru Kütüphaneyi Seçmek

Perlin Noise matematiğini sıfırdan yazmak zahmetlidir ve optimizasyon gerektirir. Voxel motorları geliştirenlerin endüstri standardı haline gelmiş, tek bir `.h` dosyasından oluşan **FastNoiseLite** kütüphanesini kullanacağız (Tıpkı `stb_image` gibi `include/` klasörüne atman yeterlidir).

## 2. Chunk İçinde Yükseklik Haritası (Heightmap) Üretimi

Chunk sınıfımızın içine dünyadaki konumunu belirtecek `chunkX` ve `chunkZ` değerlerini eklemeliyiz. Çünkü gürültü fonksiyonuna **Global (Dünya) Koordinatlarını** vermemiz gerekir, aksi takdirde her Chunk birbirinin kopyası olur.

```cpp
#include "FastNoiseLite.h"

class Chunk {
public:
    uint8_t blocks[16][256][16];
    int chunkX, chunkZ; // Bu chunk'ın dünyadaki koordinatları (Örn: 0,0 veya 1,0)

    Chunk(int x, int z) {
        chunkX = x;
        chunkZ = z;
        GenerateTerrain();
    }

    void GenerateTerrain() {
        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        
        // Arazi şekillendirme ayarları:
        // Frekans (Frequency): Haritaya ne kadar "uzaktan" baktığımızı belirler. 
        // Değer küçüldükçe dağlar genişler, büyüdükçe arazi dikenli bir hal alır.
        noise.SetFrequency(0.01f); 

        // Fraktal tipi: Dağların üzerine daha küçük girintiler/çıkıntılar ekleyerek gerçekçilik katar
        noise.SetFractalType(FastNoiseLite::FractalType_FBm);
        noise.SetFractalOctaves(4); 

        for(int x = 0; x < 16; x++) {
            for(int z = 0; z < 16; z++) {
                
                // 1. Bloğun dünyadaki GERÇEK (Global) koordinatını bul
                float globalX = (chunkX * 16.0f) + x;
                float globalZ = (chunkZ * 16.0f) + z;

                // 2. Noise değerini al (Genellikle -1.0 ile 1.0 arasında bir değer döner)
                float noiseValue = noise.GetNoise(globalX, globalZ);

                // 3. Noise değerini yüksekliğe çevir
                // Taban yüksekliğimiz 64 olsun. Üzerine noise'a göre max 30 blok dağ eklensin.
                int terrainHeight = 64 + (int)(noiseValue * 30.0f);

                // 4. Sütunu doldur
                for(int y = 0; y < 256; y++) {
                    if (y == terrainHeight) {
                        blocks[x][y][z] = 2; // En üst katman: Çimen (Örn: ID 2)
                    } else if (y < terrainHeight && y > terrainHeight - 4) {
                        blocks[x][y][z] = 1; // Altındaki 3-4 blok: Toprak (Örn: ID 1)
                    } else if (y <= terrainHeight - 4) {
                        blocks[x][y][z] = 3; // Daha derinler: Taş (Örn: ID 3)
                    } else {
                        blocks[x][y][z] = 0; // Üstü: Hava
                    }
                }
            }
        }
    }
};

```

Bu kodu çalıştırdığında, sadece tek bir Chunk içinde bile eğimli tepeler, alt katmanlarda taş, yüzeyde ise toprak katmanı oluştuğunu göreceksin.

---

## 3. Sonsuz Dünya Döngüsü (Chunk Yöneticisi)

Sonsuz dünya demek, oyuncunun hareketlerine göre etraftaki Chunk'ları yükleyip, oyuncunun uzaklaştığı Chunk'ları silmek demektir (Render Distance mantığı).

Bunun için bir **Chunk Yöneticisi (Chunk Manager)** yazmalıyız. RAM'in dolup taşmaması için oluşturulan Chunk'ları bir sözlük (`std::unordered_map`) yapısında tutmak en performanslı yöntemdir.

```cpp
#include <unordered_map>
#include <memory>

// C++'ta unordered_map için X ve Z'yi birleştiren basit bir anahtar
struct ChunkCoord {
    int x, z;
    bool operator==(const ChunkCoord& other) const {
        return x == other.x && z == other.z;
    }
};

// ... Hash fonksiyonu (Gerekli ama detayı atlıyorum) ...

class World {
public:
    std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>> loadedChunks;
    int renderDistance = 4; // Oyuncunun etrafındaki 4 chunklık yarıçap çizilir

    void Update(glm::vec3 playerPosition) {
        // Oyuncunun bulunduğu merkez Chunk'ı hesapla
        int currentChunkX = (int)(playerPosition.x / 16.0f);
        int currentChunkZ = (int)(playerPosition.z / 16.0f);

        // 1. Oyuncunun etrafındaki Chunk'ları YÜKLE
        for(int x = currentChunkX - renderDistance; x <= currentChunkX + renderDistance; x++) {
            for(int z = currentChunkZ - renderDistance; z <= currentChunkZ + renderDistance; z++) {
                
                ChunkCoord coord = {x, z};
                
                // Eğer bu chunk RAM'de (Sözlükte) yoksa, yeni oluştur!
                if (loadedChunks.find(coord) == loadedChunks.end()) {
                    loadedChunks[coord] = std::make_shared<Chunk>(x, z);
                    loadedChunks[coord]->GenerateMesh(); // Yüzeyleri hesapla
                    loadedChunks[coord]->BufferData();   // Ekran kartına gönder
                }
            }
        }

        // 2. Oyuncudan çok uzaklaşan Chunk'ları SİL (RAM'i temizle)
        // (Bu işlem için loadedChunks üzerinde döngü yapıp, 
        // currentChunkX ve Z'ye olan mesafesi renderDistance'ı aşanları erase() ile sileriz.)
    }
    
    void Render() {
        // Yüklü olan tüm chunkların çizim (glDrawArrays) fonksiyonlarını çağır
        for(auto& pair : loadedChunks) {
            // chunk->Render();
        }
    }
};

```

Ana oyun döngünde her karede (frame) `World.Update(camera.Position)` fonksiyonunu çağırdığında, sen yürüdükçe ilerideki yeni dağların ve vadilerin otomatik olarak matematiksel bir düzende oluştuğunu, arkada kalanların ise RAM'den silindiğini göreceksin.

> **Geliştirici İpucu:** Eğer performans düşüşü (anlık takılmalar) yaşarsan, Chunk üretim işlemini (özellikle Mesh oluşturma kısmını) `std::thread` kullanarak arka planda (Multi-threading) çalıştırman gerekecektir. Minecraft'ta yeni blokların yavaş yavaş yüklenmesinin sebebi budur.


# 7. soru

tamam sonraki adım olarak ne yapacağız

# 7. cevap

Sırada, oluşturduğumuz bu dağların ve vadilerin her bir bloğuna kendi kimliğini (Çimen, Taş, Toprak) görsel olarak atama işlemi var.

Ancak burada oyun motoru geliştiren herkesin düştüğü devasa bir **performans tuzağı** bulunur. Eğer OpenGL'e *"Şimdi çimen dokusunu yükle ve çiz, şimdi toprak dokusunu yükle ve çiz"* dersen, ekran kartı sürekli doku değiştirmekten (State Change) yorulur ve oyunun anında kasmaya başlar. Yüz binlerce bloğu aynı anda çizebilmemizin sırrı, ekran kartına her şeyi **tek seferde** göndermektir.

Bunu çözmek için endüstri standardı olan **Texture Atlas (Doku Atlası)** tekniğini kullanacağız.

## 1. Doku Atlası Nedir?

Elindeki Bedrock doku paketinde (Texture Pack) her blok ayrı bir `.png` dosyasıdır (`dirt.png`, `stone.png`, `grass.png` vb.). Bu ayrı dosyaları GIMP, Photoshop veya basit bir Python betiği kullanarak **tek bir büyük kare resim (Atlas)** haline getirmelisin.

Örneğin, Minecraft'ın klasik "terrain.png" dosyası gibi yan yana ve alt alta 16x16 bloktan oluşan dev bir resim (toplam 256 farklı doku kapasiteli) hazırladığını varsayalım. Bu görseli motorumuza tek bir kez yükleyeceğiz.

## 2. UV Koordinat Matematiği

Ekran kartına bir bloğun yüzeyini çizerken, o devasa Atlas resminin sadece **belirli bir karesini** kesip almasını söylemeliyiz. Buna "UV Mapping" denir. OpenGL'de resmin sol alt köşesi (0.0, 0.0), sağ üst köşesi ise (1.0, 1.0)'dır.

Eğer atlasımızda yan yana 16 blok dokusu varsa, her bir dokunun genişliği `1.0 / 16.0 = 0.0625` olur.

Blok ID'sine göre (Örn: Çimen = 2) atlas içindeki satır ve sütunu matematiksel olarak bulabiliriz:

```cpp
// 16x16'lık bir atlas kullandığımızı varsayıyoruz
float atlasSize = 16.0f;
float uvSize = 1.0f / atlasSize; // Bir bloğun atlas üzerindeki yüzdelik boyutu

int blockID = 2; // Diyelim ki Çimen bloğu

// Atlas üzerindeki Sütun (X) ve Satır (Y) konumunu bul
int col = blockID % 16; 
int row = blockID / 16;

// Keseceğimiz karenin köşe koordinatlarını hesapla
float uMin = col * uvSize;         // Sol kenar
float vMin = row * uvSize;         // Alt kenar
float uMax = uMin + uvSize;        // Sağ kenar
float vMax = vMin + uvSize;        // Üst kenar

```

## 3. Chunk Motorunu (Mesh Generator) Güncellemek

Bir önceki adımda yazdığımız `GenerateMesh()` fonksiyonunda sadece bloğun köşelerini (X, Y, Z) listeye ekliyorduk. Şimdi, çizdiğimiz o yüzeyin hangi bloğa ait olduğuna bakıp (ID), yukarıdaki hesaplamayı kullanarak UV (U, V) noktalarını da aynı listeye ekleyeceğiz.

```cpp
void GenerateMesh() {
    meshVertices.clear();

    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 256; y++) {
            for(int z = 0; z < 16; z++) {
                
                int blockID = blocks[x][y][z];
                if (blockID == 0) continue; // Hava ise atla

                // UV Koordinatlarını ID'ye göre hesapla
                float uMin = (blockID % 16) * (1.0f / 16.0f);
                float vMin = (blockID / 16) * (1.0f / 16.0f);
                float uMax = uMin + (1.0f / 16.0f);
                float vMax = vMin + (1.0f / 16.0f);

                // --- ÖN YÜZEY ÇİZİMİ (Örnek) ---
                if (/* Ön yüzey açıkta ise (Z+1 havaysa) */) {
                    
                    // 1. Üçgen (Sol Alt, Sağ Alt, Sağ Üst)
                    // Format: X, Y, Z, U, V
                    meshVertices.insert(meshVertices.end(), {
                        x-0.5f, y-0.5f, z+0.5f,  uMin, vMin, // Sol Alt Köşe
                        x+0.5f, y-0.5f, z+0.5f,  uMax, vMin, // Sağ Alt Köşe
                        x+0.5f, y+0.5f, z+0.5f,  uMax, vMax  // Sağ Üst Köşe
                    });
                    
                    // 2. Üçgen (Sağ Üst, Sol Üst, Sol Alt)
                    meshVertices.insert(meshVertices.end(), {
                        x+0.5f, y+0.5f, z+0.5f,  uMax, vMax, // Sağ Üst
                        x-0.5f, y+0.5f, z+0.5f,  uMin, vMax, // Sol Üst
                        x-0.5f, y-0.5f, z+0.5f,  uMin, vMin  // Sol Alt
                    });
                }
                
                // Arka, Üst, Alt, Sağ ve Sol yüzeyler için de aynı UV noktaları eklenir...
            }
        }
    }
}

```

Bu kodu çalıştırdığında, tek bir çizim çağrısıyla (Draw Call) binlerce blok çizilecek; ancak dağların zirvesinde çimen dokusunu, dağların eteklerinde toprak dokusunu, vadilerin diplerinde ise taş dokusunu pürüzsüz ve sıfır kasma ile görebileceksin.

> **Küçük Bir Not:** Çimen bloğunun üstü yeşil, yanları toprak görünümlüdür. Bunu yapmak için, `blockID` kontrolünde *"Eğer üst yüzeyi (Y+1) çiziyorsan ID'yi 2 al, yan yüzeyleri çiziyorsan ID'yi 1 (Toprak) olarak al"* şeklinde ufak bir "If" kontrolü eklemek yeterlidir!


# 8. soru

Tüm dünya renklendiğine göre, ekranın ortasındaki artı (crosshair) imleciyle baktığım bloğu algılayıp onu kırmayı (veya yeni blok koymayı) sağlayan Raycasting (Işın İzleme) mantığını nasıl kurarım?

# 8. Cevap

Voxel motorlarında oyuncunun dünyayla etkileşime girmesini sağlayan en kritik mekanik **Raycasting (Işın İzleme)** mantığıdır. Ekranın tam ortası, aslında 3D dünyada **kameranın baktığı yön** demektir.

Kameranın bulunduğu noktadan (gözlerimizden) ileriye doğru hayali bir çizgi (ışın) çekeriz ve bu çizgi üzerinde milim milim ilerleyerek "Burada katı bir blok var mı?" diye sorarız. Işın katı bir bloğa çarptığı an döngüyü durdururuz.

Minecraft'ta oyuncunun blok kırma/koyma menzili (reach) yaklaşık 5 bloktur. Bu kısa mesafe için en pratik ve hatasız yöntem **Step-by-Step (Adım Adım) Ray Marching** algoritmasıdır.

## 1. Işını Göndermek ve Bloğu Algılamak

Bu işlemi farenin sol veya sağ tuşuna tıklandığı an (`main.cpp` içindeki olay döngüsünde) bir kez çalıştırmalısın.

```cpp
#include <cmath>

// Fare tıklaması algılandığında çalışacak fonksiyon
void InteractWithBlock(bool isBreaking, int currentSelectedBlockID = 1) {
    
    // Kameranın konumu ve baktığı yönü alıyoruz
    glm::vec3 rayPosition = camera.Position;
    glm::vec3 rayDirection = camera.Front;
    
    float maxReach = 5.0f;     // Oyuncunun kolunun uzanabileceği maksimum blok mesafesi
    float stepSize = 0.05f;    // Işının ne kadar hassas ilerleyeceği (Çok küçük olursa kasar, çok büyük olursa blokları ıskalar)

    // Blok koymak için, ışının çarptığı bloğun "bir önceki" boş noktasını akılda tutmalıyız
    glm::vec3 lastEmptyPosition = rayPosition; 

    // Işını menzil boyunca adım adım ileri taşı
    for (float t = 0; t < maxReach; t += stepSize) {
        
        // Işının o anki 3D koordinatı
        glm::vec3 currentPoint = rayPosition + (rayDirection * t);

        // 3D noktayı, Voxel (Tam Sayı) ızgarasına yuvarla
        // std::floor kullanıyoruz çünkü negatif koordinatlarda düz (int) çevirisi hatalı sonuç verir
        int blockX = std::floor(currentPoint.x);
        int blockY = std::floor(currentPoint.y);
        int blockZ = std::floor(currentPoint.z);

        // O koordinatta bir blok var mı kontrol et (World sınıfındaki veriden çekiyoruz)
        int blockID = world.GetBlock(blockX, blockY, blockZ);

        if (blockID != 0) { 
            // IŞIN BİR BLOĞA ÇARPTI!

            if (isBreaking) {
                // SOL TIK: Bloğu Kır (ID'yi 0 yani Hava yap)
                world.SetBlock(blockX, blockY, blockZ, 0);
            } 
            else {
                // SAĞ TIK: Blok Koy (Çarptığımız bloğun bir önceki boş konumuna yeni blok koy)
                int placeX = std::floor(lastEmptyPosition.x);
                int placeY = std::floor(lastEmptyPosition.y);
                int placeZ = std::floor(lastEmptyPosition.z);
                
                world.SetBlock(placeX, placeY, placeZ, currentSelectedBlockID);
            }
            
            // İşlem bitti, ışını daha fazla uzatmanın anlamı yok
            break; 
        }

        // Eğer çarpmadıysa, burası boştur. Bir sonraki adım için burayı "son boş yer" olarak kaydet.
        lastEmptyPosition = currentPoint;
    }
}

```

## 2. Chunk Verisini Güncellemek (World Sınıfı)

Yukarıdaki kodda çağırdığımız `world.SetBlock(...)` fonksiyonu sadece RAM'deki o 3 boyutlu dizideki numarayı değiştirir. Ancak ekran kartının (OpenGL) bu değişiklikten haberi yoktur. Bloğun ekrandan kaybolması (veya görünmesi) için o Chunk'ın yüzey ağını (Mesh) yeniden hesaplatmalıyız.

World sınıfına şu fonksiyonları eklemelisin:

```cpp
void SetBlock(int globalX, int globalY, int globalZ, int blockID) {
    // 1. Global koordinattan, o bloğun hangi Chunk'ta olduğunu bul
    int chunkX = std::floor((float)globalX / 16.0f);
    int chunkZ = std::floor((float)globalZ / 16.0f);

    // 2. Bloğun Chunk içindeki yerel (Local) koordinatını bul (0 ile 15 arası)
    int localX = globalX % 16;
    if (localX < 0) localX += 16; // Negatif koordinat düzeltmesi

    int localZ = globalZ % 16;
    if (localZ < 0) localZ += 16;

    // Y ekseni (0-255 arası) her zaman pozitiftir
    if (globalY < 0 || globalY > 255) return; 

    // 3. Chunk'ı sözlükten bul
    ChunkCoord coord = {chunkX, chunkZ};
    if (loadedChunks.find(coord) != loadedChunks.end()) {
        
        // Veriyi güncelle
        loadedChunks[coord]->blocks[localX][globalY][localZ] = blockID;
        
        // 4. EKRAN KARTINI GÜNCELLE (En Önemli Adım)
        // Mesh'i baştan hesapla ve BufferData ile OpenGL'e yeni halini yolla
        loadedChunks[coord]->GenerateMesh();
        loadedChunks[coord]->BufferData(); 
    }
}

```

### Kritik Bir Optimizasyon Detayı

Eğer kırdığın blok, **Chunk'ın tam sınırındaysa** (örneğin X=15), o bloğu kırdığında yandaki (X=0) komşu Chunk'ın daha önce görünmeyen bir yüzeyi artık görünür hale gelmelidir. Kusursuz bir motor yapmak için, `SetBlock` fonksiyonunda blok sınırda kırılmışsa, yandaki Chunk'ın da `GenerateMesh()` fonksiyonunu tetiklemelisin.

# 9. Soru

İlk hedeflerimden biri olan envanter sistemine geçmek istiyorum. Kırdığım bu blokları bir listede toplayıp, arayüzde (UI) nasıl gösterebilirim?

# 9. Cevap

Envanter sistemi, motorunda **Arka Plan (Mantık)** ve **Ön Plan (Arayüz/UI)** olmak üzere iki ayrı katmanda çalışmalıdır. Kırdığın blokları RAM'de güvenle saklarken, aynı zamanda 3D dünyanın üzerine 2D bir katman (arayüz) olarak çizdirmemiz gerekir.

### 1. Envanter Veri Yapısı (Arka Plan)

Her yuvada (slot) hem bloğun türünü (ID) hem de miktarını tutacak basit bir yapıya (struct) ihtiyacımız var. Oyuncu sınıfının içine bu veri yapısını ekleyebilirsin:

```cpp
#include <vector>

struct InventorySlot {
    int blockID;
    int count;
};

class Player {
public:
    // 9 slotluk sıcak çubuk (hotbar) ve 27 slotluk ana envanter
    std::vector<InventorySlot> inventory;
    int activeSlotIndex = 0; // Oyuncunun elindeki seçili yuva

    Player() {
        // Başlangıçta 36 yuvayı boş (ID: 0) olarak doldur
        inventory.resize(36, {0, 0});
    }
};

```

### 2. Kırılan Bloğu Toplama

Bir önceki adımdaki `InteractWithBlock` fonksiyonunda bloğu kırdığımız yere (ID'yi 0 yaptığımız kısım) eşya toplama mantığını eklemelisin:

* Envanterde döngüye girip, kırılan `blockID` ile aynı olan ve 64'ten (stack sınırı) az eşya içeren bir yuva ara.
* Eğer bulursan, o yuvanın sayısını (`count`) 1 artır.
* Eğer bulamazsan, tamamen boş (`blockID == 0`) olan ilk yuvayı bul ve eşyayı oraya yerleştir.

### 3. Arayüzü (UI) Ekrana Çizdirmek

OpenGL'de 3D dünyayı çizerken derinlik hissi veren `Perspective` matrisini kullanırız. Ancak envanter barı, can kalpleri ve artı imleci (crosshair) kamerayla birlikte dönmez; ekrana yapışıktır. Bunun için çizim döngünün sonuna **Orthographic (Dik açılı) Çizim** aşamasını eklemelisin.

```cpp
// 1. 3D Dünyayı Çiz (Perspective)
shader3D.use();
shader3D.setMat4("projection", perspectiveMatrix);
world.Render();

// 2. Derinlik testini kapat (UI, 3D dünyanın içine gömülmesin)
glDisable(GL_DEPTH_TEST);

// 3. 2D Arayüzü Çiz (Orthographic)
shader2D.use();
// Ekranın sol üstünü (0,0), sağ altını (Genişlik, Yükseklik) piksel cinsinden ayarlar
glm::mat4 orthoMatrix = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
shader2D.setMat4("projection", orthoMatrix);

// Envanter arka planını ve slotlardaki eşyaların ikonlarını çiz
// DrawInventoryUI(); 

// 4. Derinlik testini tekrar aç (Bir sonraki karenin 3D çizimi için)
glEnable(GL_DEPTH_TEST);

```

`DrawInventoryUI()` aşamasında, Bedrock doku paketindeki envanter penceresini ve eşya ikonlarını, tıpkı 3D dünyadaki küplerin tek bir yüzeyini çizdiğin gibi ama bu sefer doğrudan ekran pikselleri (X, Y koordinatları) üzerine iki boyutlu düzlemler (Quad) olarak çizdireceksin.

# 10. Soru

Can barlarını (kalpleri) ve Bedrock hotbar dokusunu ekrana sabitlemek için gereken 2D Shader ve Dörtgen (Quad) çizim kodlarını nasıl yazabilirim?

# 10. Cevap

3D dünyayı çizerken derinliği (Z eksenini) kullanırız, ancak UI (Arayüz) çizerken her şeyin ekran camına yapışık, düz bir düzlemde (X ve Y eksenleri) olmasını isteriz.

Bunu en performanslı şekilde yapmak için, her arayüz elemanına (kalp, hotbar, artı imleci) ayrı ayrı köşeler (vertex) tanımlamak yerine, **sadece bir adet 1x1 boyutunda Dörtgen (Quad)** tanımlarız. Sonra bu dörtgeni matematik (Matrisler) ile büyütüp, küçültüp ekranın istediğimiz yerine kaydırırız.

İşte adım adım Minecraft arayüzünü ekrana basma işlemi:

## 1. Saydamlık (Alpha Blending) Ayarı

Minecraft'taki kalp ikonlarının veya hotbar'ın etrafı şeffaftır. Eğer bu ayarı açmazsan, ikonların etrafında iğrenç siyah kutular görürsün. UI çizimine başlamadan hemen önce OpenGL'e şeffaflığı aktif etmesini söylemelisin:

```cpp
// 3D çizim bitti, UI çizimine geçiyoruz
glDisable(GL_DEPTH_TEST); 

// Şeffaflığı (PNG'lerdeki Alpha kanalını) aktif et
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

```

---

## 2. 2D UI Shader'larını Yazmak

3D dünyadaki gölgelendirmeler veya kameralar arayüzü etkilememelidir. Bu yüzden UI için çok basit iki yeni Shader dosyası oluşturuyoruz.

**`ui_vertex.glsl` (UI Nokta Hesaplayıcı):**

```glsl
#version 330 core
layout (location = 0) in vec2 aPos;      // Sadece X ve Y (2D)
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 projection; // Orthographic (Dik açılı) matris
uniform mat4 model;      // Dörtgenin ekrandaki konumu ve boyutu

void main() {
    // Z eksenini 0.0, W'yi 1.0 olarak sabitliyoruz
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
    TexCoords = aTexCoords;
}

```

**`ui_fragment.glsl` (UI Renklendirici):**

```glsl
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D uiTexture;

void main() {
    vec4 texColor = texture(uiTexture, TexCoords);
    
    // Eğer piksel tamamen şeffafsa hiç çizme (Performans artışı)
    if(texColor.a < 0.1) discard; 
    
    FragColor = texColor;
}

```

---

## 3. Temel 1x1 Dörtgeni (Quad) Oluşturmak

Oyun başlarken (sadece bir kere) 1x1 boyutunda standart bir dörtgen oluşturup RAM'e yüklüyoruz. Bu bizim "tuvalimiz" olacak.

```cpp
unsigned int quadVAO, quadVBO;

void InitUIQuad() {
    // 0.0 ile 1.0 arasında bir kare. Sol üst köşesi (0,0)'dır.
    float vertices[] = {
        // Pozisyon (X, Y)  // Doku/UV (U, V)
        0.0f, 1.0f,         0.0f, 1.0f, // Sol Alt
        1.0f, 0.0f,         1.0f, 0.0f, // Sağ Üst
        0.0f, 0.0f,         0.0f, 0.0f, // Sol Üst

        0.0f, 1.0f,         0.0f, 1.0f, // Sol Alt
        1.0f, 1.0f,         1.0f, 1.0f, // Sağ Alt
        1.0f, 0.0f,         1.0f, 0.0f  // Sağ Üst
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Pozisyon (Atrribute 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // Doku (Attribute 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

```

---

## 4. UI Elemanlarını Ekrana Çizdiren Fonksiyon

İşte en can alıcı nokta. GLM matematiği kullanarak, elimizdeki o 1x1'lik beyaz tuvali ekranda istediğimiz boyuta sündürüp içine Bedrock dokularımızı basıyoruz.

```cpp
// position: Ekrandaki X, Y piksel konumu (Sol üstten başlar)
// size: Genişlik ve Yükseklik (Piksel cinsinden)
void DrawUIElement(unsigned int textureID, glm::vec2 position, glm::vec2 size) {
    shader2D.use();
    
    // Matrisi sıfırla (Birim Matris)
    glm::mat4 model = glm::mat4(1.0f);
    
    // 1. Önce istenilen konuma kaydır (Translate)
    model = glm::translate(model, glm::vec3(position, 0.0f));  
    
    // 2. Sonra istenilen boyuta büyüt (Scale)
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader2D.setMat4("model", model);
    
    // Dokuyu bağla ve çiz
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

```

---

## 5. Render Döngüsünde Kalpleri ve Hotbar'ı Çizmek

Oyun döngünde `glDisable(GL_DEPTH_TEST)` dedikten hemen sonra bu fonksiyonu kullanarak arayüzünü (Örn: 800x600 çözünürlükte) oluşturabilirsin:

```cpp
// Kameranın orthographic ayarı (Ekran çözünürlüğüne göre)
glm::mat4 ortho = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
shader2D.setMat4("projection", ortho);

// 1. HOTBAR ÇİZİMİ (Ekranın en alt ortasına sabitleyelim)
float hotbarWidth = 400.0f; // Hotbar'ın ekrandaki genişliği
float hotbarHeight = 44.0f; 
float hotbarX = (800.0f / 2.0f) - (hotbarWidth / 2.0f); // Ekranı ortala
float hotbarY = 600.0f - hotbarHeight;                  // En alta yapıştır

DrawUIElement(hotbarTexture, glm::vec2(hotbarX, hotbarY), glm::vec2(hotbarWidth, hotbarHeight));

// 2. CAN BARLARI (KALPLER) (Hotbar'ın hemen üstüne çizelim)
// 10 adet kalp çiziyoruz
float heartSize = 18.0f;
float heartStartX = hotbarX; // Hotbar ile aynı hizadan başlasın
float heartY = hotbarY - heartSize - 5.0f; // Hotbar'ın 5 piksel üstünde

for(int i = 0; i < 10; i++) {
    DrawUIElement(heartTexture, glm::vec2(heartStartX + (i * heartSize), heartY), glm::vec2(heartSize, heartSize));
}

// 3. ARTI İMLECİ (CROSSHAIR) (Ekranın tam ortasına)
DrawUIElement(crosshairTexture, glm::vec2(390.0f, 290.0f), glm::vec2(20.0f, 20.0f));

```

Bu kodu çalıştırdığında 3D dağların üzerinde muazzam bir netlikle (Texture filtering'i GL_NEAREST yaptığımızı unutma) Bedrock arayüzünün oturduğunu göreceksin!


# 11. Soru

Harika, arayüz iskeleti oturdu. Kırdığım ve envanterimde biriken toprak, taş gibi blokların ikonlarını bu Hotbar yuvalarının içine (slotlara) nasıl çizebilirim? 

# 11. Cevap

Arayüzün arka planını çizdirdiğimize göre, işin en keyifli kısmı olan "toplanan ganimetleri ekranda gösterme" aşamasına geldik.

Burada çözmemiz gereken ufak bir teknik pürüz var: Can barları veya Hotbar gibi arayüz dosyaları tekil `.png` dosyalarıdır. Ancak topladığımız blokların (Toprak, Taş) ikonlarını çizerken daha önce 3D dünya için oluşturduğumuz **Doku Atlası'nı (Texture Atlas)** kullanmalıyız.

Eğer az önce yazdığımız `DrawUIElement` fonksiyonuna Atlas'ı doğrudan verirsek, slotun içine 256 bloğun hepsini birden sıkıştırarak çizer. Arayüz Gölgelendiricimize (UI Shader), *"Bu resmin sadece şu koordinatlarındaki kareyi kes ve çiz"* diyebilmemiz gerekiyor.

Bunu yapmak için sistemimizi ufak bir dokunuşla güncelleyelim:

### 1. UI Shader'a UV Kesme (Cropping) Yeteneği Eklemek

`ui_vertex.glsl` dosyanı aç ve dışarıdan bir `uvOffset` (UV Kaydırma) parametresi alacak şekilde şu güncellemeyi yap:

```glsl
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;

// x = uMin, y = vMin, z = uBoyut, w = vBoyut
uniform vec4 uvOffset; 

void main() {
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
    
    // Orijinal (0.0 - 1.0) değerlerini atlasın istediğimiz bölgesine sıkıştırıyoruz
    TexCoords = (aTexCoords * vec2(uvOffset.z, uvOffset.w)) + vec2(uvOffset.x, uvOffset.y);
}

```

Böylece normal UI çizimlerinde `uvOffset` değerini `(0.0, 0.0, 1.0, 1.0)` vererek resmin tamamını çizebiliriz. Atlas'tan blok çizerken ise ilgili bloğun UV sınırlarını veririz.

### 2. Çizim Fonksiyonunu Güncellemek

C++ tarafında yazdığımız fonksiyonu, bu yeni Shader yapısını destekleyecek şekilde genişletelim:

```cpp
void DrawUIElement(unsigned int textureID, glm::vec2 position, glm::vec2 size, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)) {
    shader2D.use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader2D.setMat4("model", model);
    
    // Shader'a UV kesim sınırlarını gönder
    shader2D.setVec4("uvOffset", uvRect); 
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

```

### 3. Slot İçine İkonları Çizmek (Render Döngüsü)

Hotbar'ı çizdirdiğin kodun hemen altına, `player.inventory` listesinin ilk 9 elemanını (Hotbar yuvalarını) döngüye sokarak blok ikonlarını çizdirelim:

```cpp
// 1. Önce Hotbar'ın arka planını çiz (Tam resim kullanılıyor)
DrawUIElement(hotbarTexture, glm::vec2(hotbarX, hotbarY), glm::vec2(hotbarWidth, hotbarHeight));

// Hotbar'ın içindeki yuvaların hizalaması (Göz kararı / Piksel hesabıyla)
// Hotbar 400 piksel genişliğindeyse, her bir yuva yaklaşık 44 piksel aralıklıdır
float slotStartX = hotbarX + 8.0f; // Sol kenardan ufak bir boşluk
float slotY = hotbarY + 6.0f;      // Üst kenardan ufak bir boşluk
float slotStep = (hotbarWidth - 16.0f) / 9.0f; // 9 yuva arası mesafe
float iconSize = 28.0f;            // İkonun boyutu (Yuvadan biraz küçük)

// 2. Envanterin ilk 9 yuvasını kontrol et
for(int i = 0; i < 9; i++) {
    int blockID = player.inventory[i].blockID;
    
    // Eğer yuvada bir eşya varsa (0 = Hava/Boş)
    if (blockID != 0) {
        // 3D dünyada yaptığımız UV hesabının aynısı
        float uvSize = 1.0f / 16.0f; // Atlasın 16x16 olduğunu varsayıyoruz
        float uMin = (blockID % 16) * uvSize;
        float vMin = (blockID / 16) * uvSize;
        
        // Bu bloğun ekrandaki yerini hesapla
        float currentSlotX = slotStartX + (i * slotStep);
        
        // Atlastan sadece bu bloğun yüzeyini kesip, arayüze yapıştır!
        DrawUIElement(blockAtlasTexture, 
                      glm::vec2(currentSlotX, slotY), 
                      glm::vec2(iconSize, iconSize), 
                      glm::vec4(uMin, vMin, uvSize, uvSize)); // Sadece X, Y ve Genişlik/Yükseklik
    }
}

```

### 4. Seçili Yuva (Aktif Slot) Çerçevesi

Oyunculuk deneyimi için oyuncunun klavyeden 1'den 9'a kadar bir tuşa bastığında (veya fare tekerleğini çevirdiğinde) hangi yuvayı seçtiğini bilmesi gerekir. Bedrock paketindeki o beyaz, kalın seçim çerçevesini (selector) en üste çizeriz.

```cpp
// Seçili yuva indeksini (0 ile 8 arası) oyuncunun tuş girişlerine göre güncellediğini varsayıyoruz
float activeX = slotStartX + (player.activeSlotIndex * slotStep) - 4.0f; // Çerçeve ikondan biraz büyüktür
float activeY = slotY - 4.0f;
float selectorSize = 36.0f;

// Seçim çerçevesini (selector.png) en son çiz ki ikonların altında kalmasın
DrawUIElement(selectorTexture, glm::vec2(activeX, activeY), glm::vec2(selectorSize, selectorSize));

```

Artık 3D dünyada bir toprağı veya taşı kırdığında, arayüzdeki ilgili slota o bloğun simgesinin anında yerleştiğini göreceksin. Hatta fare tekerleğiyle yuvalar arasında gezindiğinde beyaz çerçeve de hareket edecek!


# 12. Soru

Hotbar'daki eşyaların sağ alt köşesinde kaç adet (Örn: 64) olduklarını gösteren sayıları C++ ve OpenGL ile ekrana nasıl yazdırabilirim?

# 12. Cevap

OpenGL'de ekrana yazı (text) yazdırmak, oyun programlamaya yeni başlayanların en çok zorlandığı konulardan biridir. Çünkü OpenGL'in "buraya 64 yaz" diyebileceğin hazır bir fonksiyonu yoktur; o sadece üçgenleri ve pikselleri bilir.

Genellikle bunun için `FreeType` gibi ağır kütüphaneler (TTF dosyalarını okumak için) kullanılır. Ancak sen bir Minecraft klonu (Voxel motoru) yapıyorsun! Bu senin için büyük bir şans çünkü **Bitmap Font (Doku Haritası Yazı Tipi)** denilen, bizim az önce kurduğumuz altyapıyla %100 uyumlu ve çok daha performanslı olan retro yöntemi kullanabilirsin.

### 1. Bedrock Font Dokusunu (Ascii.png) Bulmak

Bedrock doku paketinin içinde genellikle `font/default8.png`, `font/ascii.png` veya `font/glyph_xx.png` adında bir görsel bulunur. Bu görseli açarsan, tüm harflerin ve sayıların **16x16'lık bir ızgara (grid)** içine dizildiğini görürsün. Toplam 256 karakter vardır.

Bu bizim için harika bir haber çünkü klavyedeki her bir tuşun (ASCII tablosu) bilgisayarda 0 ile 255 arasında bir sayısal karşılığı vardır. Örneğin `'0'` karakterinin ASCII değeri 48'dir.

Bu font dosyasını tıpkı UI ikonları gibi `stb_image` ile motoruna yükle (`fontTexture`).

### 2. Yazı Çizdirme (RenderText) Fonksiyonu

Elimizde zaten Doku Atlası'ndan belirli bir parçayı kesip ekrana basan (UV Offset) harika bir `DrawUIElement` fonksiyonumuz var. Tek yapmamız gereken bir kelimeyi (örn: "64") harf harf döngüye sokup, her harfin ASCII değerine göre font resminden ilgili kareyi kesmek.

Motoruna şu fonksiyonu ekle:

```cpp
#include <string>

// text: Ekrana yazılacak kelime veya sayı
// startX, startY: Yazının sol üst başlangıç konumu
// scale: Yazının büyüklüğü
void RenderText(std::string text, float startX, float startY, float scale, unsigned int fontTexture) {
    
    // Orijinal Minecraft fontları genellikle 8x8 piksel tasarlanır
    float charSize = 8.0f * scale; 
    float uvSize = 1.0f / 16.0f; // 16x16 grid

    float currentX = startX;

    for (char c : text) {
        int ascii = (int)c; // Karakterin numarasını al (Örn: '6' -> 54)

        // ASCII değerine göre resimdeki sütun ve satırı bul
        int col = ascii % 16;
        int row = ascii / 16; 

        // Eğer resmin yüklenirken ters dönmüşse (stb_image varsayılanı)
        // row hesabını ters çevirmen gerekebilir: row = 15 - (ascii / 16);

        float uMin = col * uvSize;
        float vMin = row * uvSize;

        // Harfi ekrana çiz!
        DrawUIElement(fontTexture, 
                      glm::vec2(currentX, startY), 
                      glm::vec2(charSize, charSize), 
                      glm::vec4(uMin, vMin, uvSize, uvSize));

        // Bir sonraki harf için X pozisyonunu sağa kaydır
        // Harfler birbirine çok yapışmasın diye biraz boşluk (charSize * 0.8f) bırakıyoruz
        currentX += (charSize * 0.8f); 
    }
}

```

### 3. Hotbar Döngüsüne Entegre Etmek

Şimdi tek yapmamız gereken, bir önceki adımda yazdığımız Hotbar çizim döngüsünün içine girip, eğer eşya sayısı 1'den büyükse (Minecraft'ta 1 tane eşya varsa sayı yazmaz) bu sayıyı sağ alt köşeye yazdırmak.

`std::to_string` kullanarak C++'taki tam sayıları (`int`), `RenderText`'in istediği yazıya (`std::string`) çevirebilirsin:

```cpp
for(int i = 0; i < 9; i++) {
    int blockID = player.inventory[i].blockID;
    int count = player.inventory[i].count; // Slot'taki eşya sayısı
    
    if (blockID != 0) {
        float uvSize = 1.0f / 16.0f; 
        float uMin = (blockID % 16) * uvSize;
        float vMin = (blockID / 16) * uvSize;
        
        float currentSlotX = slotStartX + (i * slotStep);
        
        // 1. İkonu Çiz
        DrawUIElement(blockAtlasTexture, 
                      glm::vec2(currentSlotX, slotY), 
                      glm::vec2(iconSize, iconSize), 
                      glm::vec4(uMin, vMin, uvSize, uvSize));
                      
        // 2. Eğer sayı 1'den fazlaysa, sağ alt köşeye sayıyı yaz!
        if (count > 1) {
            std::string countText = std::to_string(count);
            
            // Sayının pozisyonunu ayarla (Slot'un sağ alt köşesine hizala)
            // Yazının uzunluğuna göre X'i biraz daha sola çekmemiz gerekebilir (Örn: "9" vs "64")
            float textScale = 2.0f;
            float textWidth = countText.length() * (8.0f * textScale * 0.8f);
            
            float textX = currentSlotX + iconSize - textWidth; // Sağ yaslama mantığı
            float textY = slotY + iconSize - (8.0f * textScale); // Alt yaslama
            
            RenderText(countText, textX, textY, textScale, fontTexture);
        }
    }
}

```

**Grafiksel Bir İpucu:** Minecraft'taki yazıların çok net okunmasının sebebi, her sayının arkasında aslında kendisinin siyah ve bir tık kaydırılmış bir "gölgesi (Drop Shadow)" olmasıdır. Eğer daha profesyonel görünmesini istersen, `RenderText`'i asıl renkle çağırmadan hemen önce aynı sayıyı siyah renk (veya koyu gri bir font) ile `textX + 2.0f, textY + 2.0f` konumuna çizdirebilirsin!

# 13. soru

Tüm temel mekanikler oturdu ama dünya şu an çok düz görünüyor. Küplere 3D hissini vermek ve derinlik katmak için basit yüzey ışıklandırmasını ve ortam karartmasını (Ambient Occlusion) nasıl ekleyebilirim?

# 13. Cevap

Voxel oyunlarında dünyayı üç boyutlu ve gerçekçi gösteren şey, ekran kartının dinamik ışıklandırma hesaplamaları (Ray Tracing vb.) değildir. Çünkü milyonlarca küp için gerçek zamanlı ışık hesaplamak en güçlü sistemleri bile yorar.

Bunun yerine, Minecraft'ın da kullandığı ve tamamen **Chunk üretimi (Mesh Generation) aşamasında hesaplanan, GPU'yu hiç yormayan "Fırınlanmış" (Baked) Işıklandırma** tekniğini kullanacağız. Bu iki aşamadan oluşur: Yüzey Işıklandırması (Fake Sun) ve Ortam Karartması (Voxel Ambient Occlusion).

### 1. Yüzey Işıklandırması (Face Lighting)

Bir küpün tüm yüzeyleri aynı parlaklıkta çizilirse, gözümüz onu 3 boyutlu bir küp olarak değil, düz bir altıgen gibi algılar. Güneşin her zaman tam tepeden vurduğunu varsayarak, yüzeylerin yönüne göre sabit bir gölgeleme çarpanı belirleyeceğiz.

* **Üst Yüzey (Y+1):** %100 Parlaklık (Çarpan: `1.0f`)
* **Ön / Arka Yüzeyler (Z):** %80 Parlaklık (Çarpan: `0.8f`)
* **Sağ / Sol Yüzeyler (X):** %60 Parlaklık (Çarpan: `0.6f`)
* **Alt Yüzey (Y-1):** %40 Parlaklık (Çarpan: `0.4f`)

### 2. Ortam Karartması (Ambient Occlusion - AO)

AO, iki bloğun birleştiği iç köşelerde oluşan o yumuşak, gerçekçi gölgedir. Bunu hesaplamak için, küpün çizdiğimiz yüzeyindeki **her bir köşenin (vertex) etrafındaki 3 komşu bloğun dolu olup olmadığına** bakarız.

Bir köşe (Vertex) için AO değeri 0 (En Karanlık) ile 3 (En Aydınlık) arasında değişir:

* Etrafındaki 3 blok da hava ise = **3** (Aydınlık)
* 1 blok doluysa = **2** (Hafif gölge)
* 2 blok doluysa = **1** (Koyu gölge)
* Yanlardaki 2 blok doluysa köşenin bir önemi kalmaz = **0** (Tam karanlık)

---

### Kodlara Entegre Etmek

Bu iki veriyi ekran kartına göndermek için `GenerateMesh()` fonksiyonunda listeye eklediğimiz köşe (Vertex) formatını genişletmeliyiz. Artık her noktamız 5 değil, **6 veriden** (X, Y, Z, U, V, Gölgelendirme) oluşacak. Gölgelendirme verisine hem Yüzey Işığını hem de AO değerini çarpıp tek bir `float` olarak göndereceğiz ki ekran kartı hiç yorulmasın.

`Chunk` sınıfında, üst yüzeyi (Y+1) çizen kısmı şu şekilde güncelleyebiliriz:

```cpp
// Belirli bir koordinatta katı bir blok var mı kontrol eden yardımcı fonksiyon
bool IsSolid(int x, int y, int z) {
    if (x < 0 || x >= 16 || y < 0 || y >= 256 || z < 0 || z >= 16) return false;
    return blocks[x][y][z] != 0;
}

// AO değerini (0, 1, 2, 3) hesaplayan fonksiyon
int VertexAO(bool side1, bool side2, bool corner) {
    if (side1 && side2) return 0; // İki yan da doluysa tam karanlık
    return 3 - (side1 + side2 + corner);
}

```

Mesh üretimi içindeki Üst Yüzey (Top Face) Çizimi:

```cpp
// Eğer üst yüzey (Y+1) açıktaysa:
if (y == 255 || blocks[x][y+1][z] == 0) {
    
    float faceLight = 1.0f; // Üst yüzey güneşi tam alır
    
    // Üst yüzeyin 4 köşesi için komşu blokları kontrol et (AO Hesaplaması)
    
    // 1. Sol Alt Köşe İçin Komşular (X-1, Z+1)
    int ao00 = VertexAO(IsSolid(x-1, y+1, z), IsSolid(x, y+1, z+1), IsSolid(x-1, y+1, z+1));
    // 2. Sağ Alt Köşe İçin (X+1, Z+1)
    int ao10 = VertexAO(IsSolid(x+1, y+1, z), IsSolid(x, y+1, z+1), IsSolid(x+1, y+1, z+1));
    // 3. Sağ Üst Köşe İçin (X+1, Z-1)
    int ao11 = VertexAO(IsSolid(x+1, y+1, z), IsSolid(x, y+1, z-1), IsSolid(x+1, y+1, z-1));
    // 4. Sol Üst Köşe İçin (X-1, Z-1)
    int ao01 = VertexAO(IsSolid(x-1, y+1, z), IsSolid(x, y+1, z-1), IsSolid(x-1, y+1, z-1));

    // AO Değerlerini (0-3) renk çarpanına çevir (Örn: 3 -> 1.0, 0 -> 0.4)
    float aoMultiplier[4] = { 0.4f, 0.6f, 0.8f, 1.0f };

    // Her köşe için nihai gölgelendirmeyi hesapla (Yüzey Işığı * AO)
    float shade00 = faceLight * aoMultiplier[ao00];
    float shade10 = faceLight * aoMultiplier[ao10];
    float shade11 = faceLight * aoMultiplier[ao11];
    float shade01 = faceLight * aoMultiplier[ao01];

    // Üçgenleri listeye eklerken artık 6. parametre olarak Shade (Gölge) değerini veriyoruz
    meshVertices.insert(meshVertices.end(), {
        // X,      Y,      Z,       U,     V,     Gölge Değeri
        x-0.5f, y+0.5f, z+0.5f,  uMin, vMin,  shade00, // Sol Alt
        x+0.5f, y+0.5f, z+0.5f,  uMax, vMin,  shade10, // Sağ Alt
        x+0.5f, y+0.5f, z-0.5f,  uMax, vMax,  shade11, // Sağ Üst
        
        x+0.5f, y+0.5f, z-0.5f,  uMax, vMax,  shade11, // Sağ Üst
        x-0.5f, y+0.5f, z-0.5f,  uMin, vMax,  shade01, // Sol Üst
        x-0.5f, y+0.5f, z+0.5f,  uMin, vMin,  shade00  // Sol Alt
    });
}

```

*(Not: Bu işlemi yan ve alt yüzeyler için de kendi komşu koordinatlarına göre uyarlamalısın.)*

---

### 3. Shader (Gölgelendirici) Güncellemesi

Artık C++ tarafından ekran kartına, "bu pikseli şu kadar karart" diyen hazır bir matematiksel sonuç gönderiyoruz. `BufferData` fonksiyonunda ekran kartına (VAO) bu yeni 6. parametreyi tanıttıktan sonra (Attribute 2), Shader dosyalarını şöyle güncellemelisin:

**`vertex.glsl` (3D Nokta Hesaplayıcı):**

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aShade; // C++'tan gelen yeni gölge değeri

out vec2 TexCoord;
out float Shade; // Fragment shader'a aktar

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Shade = aShade;
}

```

**`fragment.glsl` (Renk Hesaplayıcı):**

```glsl
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float Shade; // Vertex shader'dan gelen değer

uniform sampler2D ourTexture;

void main() {
    // Orijinal doku rengini al
    vec4 texColor = texture(ourTexture, TexCoord);
    
    // Rengi gelen gölge değeriyle çarp (Sadece RGB kanallarını çarpıyoruz, Alpha (Saydamlık) bozulmasın)
    FragColor = vec4(texColor.rgb * Shade, texColor.a);
}

```

Motoru tekrar derleyip çalıştırdığında, blokların artık dümdüz bir duvar gibi görünmediğini; güneşin belli bir açıdan vurduğunu ve mağaralara girerken veya blokların iç köşelerine bakarken çok yumuşak, harika bir gölgeleme oluştuğunu göreceksin.

# 14. soru

Grafiklerimiz oturduğuna göre, yeni Chunk'lar yüklenirken oyunun anlık olarak donmasını (takılmasını) engellemek için Multi-threading (Çoklu İş Parçacığı) mimarisini nasıl kurabilirim?

# 14. Cevap

Voxel motorlarında "Takılma" (Lag Spike) sorununun ana kaynağı, Perlin Noise ile milyonlarca bloğun hesaplanması ve ardından bunların dış yüzeylerinin (Mesh) bulunması işleminin çok ciddi bir CPU gücü istemesidir. Eğer bu işlemi ana oyun döngünde yaparsan, işlem bitene kadar ekran donar (FPS sıfıra düşer).

Bunu çözmek için **Multi-threading (Çoklu İş Parçacığı)** kullanacağız. Ancak burada, grafik programlamanın **"Altın Kuralı"** ile karşılaşıyoruz:

> **Altın Kural:** Hiçbir OpenGL fonksiyonu (`glGenBuffers`, `glBindVertexArray` vb.) arka plandaki bir iş parçacığında çalıştırılamaz! OpenGL sadece pencerenin oluşturulduğu ana iş parçacığında (Main Thread) yaşar. Eğer arka planda ekran kartına veri göndermeye çalışırsan oyun anında çöker.

Bu yüzden mimariyi ikiye bölmeliyiz:

1. **Arka Plan (İşçi) İş Parçacığı:** Dağları oluşturur ve dış yüzeyleri (Mesh) hesaplar.
2. **Ana (Çizim) İş Parçacığı:** Hesabı biten Chunk'ları alır ve ekran kartına (BufferData) gönderir.

İşte bu sistemi C++'ta güvenle kurmanın yolu:

### 1. Güvenli Bir Kuyruk (Thread-Safe Queue) Oluşturmak

Arka plandaki işçiler işlerini bitirdiğinde, bu Chunk'ları ana ipliğe haber vermek için bir kuyruğa koymalıdır. İki farklı iş parçacığının aynı listeye aynı anda erişip programı çökertmemesi için `std::mutex` (Kilit) kullanmalıyız.

`World` sınıfına şu değişkenleri ekle:

```cpp
#include <thread>
#include <mutex>
#include <queue>

class World {
public:
    std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>> loadedChunks;
    
    // İşlemi biten ve ekran kartına gönderilmeyi bekleyen Chunk'ların kuyruğu
    std::queue<std::shared_ptr<Chunk>> chunksToBuffer;
    
    // Kuyruğa aynı anda erişimi engellemek için kilit
    std::mutex queueMutex; 

    // ... diğer değişkenler ...
};

```

### 2. Chunk Yükleme Mantığını Değiştirmek

Eskiden `World.Update()` içinde Chunk'ı oluşturur oluşturmaz ekran kartına gönderiyorduk. Şimdi bu işi bir `std::thread` içine hapsedeceğiz.

`World::Update` fonksiyonundaki Chunk üretme kısmını şu şekilde değiştir:

```cpp
// Eğer bu chunk RAM'de yoksa, yeni oluştur!
if (loadedChunks.find(coord) == loadedChunks.end()) {
    
    // Chunk'ı RAM'de oluştur (Sözlüğe ekle)
    auto newChunk = std::make_shared<Chunk>(x, z);
    loadedChunks[coord] = newChunk;

    // AĞIR İŞLEMİ ARKA PLANA (YENİ BİR İPLİĞE) AT
    std::thread([this, newChunk]() {
        
        newChunk->GenerateTerrain(); // Perlin Noise (Ağır işlem)
        newChunk->GenerateMesh();    // Yüzey hesaplama (Ağır işlem)

        // İşlem bitti! Chunk'ı hazır kuyruğuna ekle
        // Kilit (lock_guard), bu süslü parantez bitene kadar kuyruğu diğer ipliklere kapatır
        {
            std::lock_guard<std::mutex> lock(this->queueMutex);
            this->chunksToBuffer.push(newChunk);
        }

    }).detach(); // detach(), ipliği serbest bırakır (kendi kendine kapanır)
}

```

### 3. Ana Döngüde GPU'yu Beslemek (BufferData)

Arka plandaki işçiler çalışırken ana oyun döngün saniyede 60-144 kare hızında dönmeye devam eder. Her karede, "Acaba arka plandaki işçiler bana yeni bir Chunk hazırlamış mı?" diye kuyruğa bakmalısın.

Yine `World::Update()` fonksiyonunun en sonuna (Veya `Render` fonksiyonundan hemen önceye) şu kodu ekle:

```cpp
// Hazır olan Chunk'ları ekran kartına gönder
{
    // Kuyruğa erişeceğimiz için tekrar kilitliyoruz
    std::lock_guard<std::mutex> lock(queueMutex);
    
    // Performans için her karede en fazla 1 veya 2 Chunk yükle
    // (Eğer kuyrukta 100 chunk varsa hepsini aynı karede yüklemek yine anlık takılma yapar)
    int chunksBufferedThisFrame = 0;
    
    while (!chunksToBuffer.empty() && chunksBufferedThisFrame < 2) {
        
        auto readyChunk = chunksToBuffer.front(); // Kuyruğun en başındakini al
        chunksToBuffer.pop();                     // Kuyruktan çıkar
        
        readyChunk->BufferData(); // EKRAN KARTINA GÖNDER (Sadece ana iş parçacığında çalışır!)
        
        chunksBufferedThisFrame++;
    }
}

```

### 4. Ekstra Bir Güvenlik: Chunk Durumları (State)

Eğer bir Chunk yüklenirken oyuncu o yönden vazgeçip geri dönerse ve Chunk'ı silmeye çalışırsan, arka plandaki iş parçacığı silinmiş bir veriye erişip "Segmentation Fault" hatası verebilir.

Bunu önlemek için Chunk sınıfına basit bir durum makinesi (Enum) eklemen hayat kurtarır:

```cpp
enum ChunkState { 
    EMPTY,        // Yeni oluşturuldu, içi boş
    GENERATING,   // Şu an arka planda thread içinde hesaplanıyor
    READY,        // Hesaplandı, BufferData bekliyor
    ACTIVE        // Sahnede çiziliyor
};

// Çizim döngüsünde (Render), sadece durumu ACTIVE olanları çizersin:
if (chunk->state == ACTIVE) {
    // Çizim kodları...
}

```

Bu sistemi kurduktan sonra oyunda ileriye doğru koşmaya başladığında, FPS'nin **hiç düşmediğini**, dağların ve vadilerin senin hızına yetişmek için saniyeler içinde yumuşak bir şekilde ufukta belirdiğini göreceksin! Gerçek bir oyun motoru hissiyatı tam olarak burada başlar.

# 15. soru

Oyuncu sadece ileriye bakarken, arkasında kalan binlerce bloğu ekran kartının boşuna çizmesini engelleyerek devasa bir performans artışı sağlayan Frustum Culling (Görüş Alanı Kırpma) sistemini nasıl entegre edebilirim?

# 15. cevap

Ekran kartları, oyuncunun görmediği şeyleri çizmeme konusunda kendi içlerinde bazı optimizasyonlara (örneğin az önce kurduğumuz Face Culling) sahiptir. Ancak bu optimizasyonlar, veriler ekran kartına **gönderildikten sonra** çalışır. Yani ekran kartı arkanda kalan milyonlarca bloğun verisini yine de işler ve sonra çöpe atar.

Frustum Culling (Görüş Alanı Kırpma) ise CPU (İşlemci) tarafında çalışan bir matematiktir. Mantığı şudur: *"Eğer bu Chunk kameranın görüş açısının (Frustum) tamamen dışındaysa, ekran kartına (glDrawArrays) hiç gönderme!"*

Bunu başardığında oyununun FPS'i Render Distance'ı (Görüş Mesafesini) 16 veya 32 Chunk yapsan bile tavan yapacaktır.

### 1. Frustum (Görüş Piramidi) Nedir?

Kameran dünyayı düz bir çizgi halinde görmez. Gözünden çıkan ve ufka doğru genişleyen, piramit benzeri bir geometrik şekil (Frustum) içinde kalanları görürsün. Bu piramidin 6 adet yüzeyi (Düzlemi/Plane) vardır: **Sol, Sağ, Üst, Alt, Yakın, Uzak.**

Eğer bir Chunk'ın sınırlayıcı kutusu (AABB - Axis Aligned Bounding Box) bu 6 yüzeyin herhangi birinin **tamamen dışındaysa**, o Chunk ekranda değildir.

### 2. Matematiksel Düzlem (Plane) Yapısı

Önce projemizde matematiksel bir "Düzlem" yapısı oluşturmalıyız. `include/Camera.h` dosyanı aç ve en üste şu yapıyı ekle:

```cpp
struct Plane {
    glm::vec3 normal; // Düzlemin baktığı yön
    float distance;   // Merkeze olan uzaklığı

    // Düzlemin matematiğini normalize eden (birim vektöre çeviren) fonksiyon
    void Normalize() {
        float length = glm::length(normal);
        normal /= length;
        distance /= length;
    }
};

```

### 3. Kameranın İçine Kırpma Sistemini Eklemek

`Camera` sınıfının içine bu 6 düzlemi hesaplayan ve bir kutunun (Chunk'ın) görünür olup olmadığını test eden mekanizmayı kuruyoruz. Bu test çok hızlı çalışır.

```cpp
class Camera {
public:
    // Mevcut kamera değişkenlerin... (Position, Front, Up vb.)
    
    Plane frustumPlanes[6]; // Kameranın 6 görüş yüzeyi

    // Her karede (veya kamera her hareket ettiğinde) çağrılacak
    void UpdateFrustum(glm::mat4 projectionMatrix) {
        
        // Görüş matrisi (View) ile Projeksiyon matrisini birleştir
        glm::mat4 vp = projectionMatrix * GetViewMatrix();

        // OpenGL matrislerinden 6 yüzeyin denklemlerini (Sol, Sağ, Alt, Üst, Yakın, Uzak) çıkarıyoruz:

        // 1. Sol Yüzey
        frustumPlanes[0].normal = glm::vec3(vp[0][3] + vp[0][0], vp[1][3] + vp[1][0], vp[2][3] + vp[2][0]);
        frustumPlanes[0].distance = vp[3][3] + vp[3][0];

        // 2. Sağ Yüzey
        frustumPlanes[1].normal = glm::vec3(vp[0][3] - vp[0][0], vp[1][3] - vp[1][0], vp[2][3] - vp[2][0]);
        frustumPlanes[1].distance = vp[3][3] - vp[3][0];

        // 3. Alt Yüzey
        frustumPlanes[2].normal = glm::vec3(vp[0][3] + vp[0][1], vp[1][3] + vp[1][1], vp[2][3] + vp[2][1]);
        frustumPlanes[2].distance = vp[3][3] + vp[3][1];

        // 4. Üst Yüzey
        frustumPlanes[3].normal = glm::vec3(vp[0][3] - vp[0][1], vp[1][3] - vp[1][1], vp[2][3] - vp[2][1]);
        frustumPlanes[3].distance = vp[3][3] - vp[3][1];

        // 5. Yakın Yüzey
        frustumPlanes[4].normal = glm::vec3(vp[0][3] + vp[0][2], vp[1][3] + vp[1][2], vp[2][3] + vp[2][2]);
        frustumPlanes[4].distance = vp[3][3] + vp[3][2];

        // 6. Uzak Yüzey
        frustumPlanes[5].normal = glm::vec3(vp[0][3] - vp[0][2], vp[1][3] - vp[1][2], vp[2][3] - vp[2][2]);
        frustumPlanes[5].distance = vp[3][3] - vp[3][2];

        // Tüm düzlemleri normalize et (Matematiksel hata olmaması için şart)
        for (int i = 0; i < 6; i++) {
            frustumPlanes[i].Normalize();
        }
    }

    // Bir Chunk'ın görünür olup olmadığını kontrol eden fonksiyon
    bool IsBoxVisible(glm::vec3 minBox, glm::vec3 maxBox) {
        
        // Kutunun tüm köşelerini 6 düzlemle test et
        for (int i = 0; i < 6; i++) {
            
            // Kutunun, düzlemin normaline (yönüne) en yakın olan "Pozitif" köşesini bul
            glm::vec3 positiveVertex;
            positiveVertex.x = (frustumPlanes[i].normal.x > 0) ? maxBox.x : minBox.x;
            positiveVertex.y = (frustumPlanes[i].normal.y > 0) ? maxBox.y : minBox.y;
            positiveVertex.z = (frustumPlanes[i].normal.z > 0) ? maxBox.z : minBox.z;

            // Eğer bu köşe düzlemin arkasında (dışında) kalıyorsa, kutu kesinlikle görünmezdir!
            if (glm::dot(frustumPlanes[i].normal, positiveVertex) + frustumPlanes[i].distance < 0.0f) {
                return false; 
            }
        }
        
        return true; // Hiçbir düzlemin dışında değilse, görünürdür.
    }
};

```

### 4. Çizim (Render) Döngüsüne Entegre Etmek

Bu sistemi kurduktan sonra `World::Render` fonksiyonumuzda harika bir kestirme yol yaratıyoruz. Sadece Chunk'ın dünya üzerindeki minimum (Sol alt) ve maksimum (Sağ üst) koordinatlarını hesaplayıp `Camera`'ya soracağız.

Bir Chunk 16x256x16 boyutlarında bir kutudur. (Başlangıçta dünyayı 256 blok yükseklik üzerinden kurguladığımızı hatırlayalım).

```cpp
void Render(Camera& camera, glm::mat4 projectionMatrix) {
    
    // Kameranın görüş piramidini, oyuncu her hareket ettiğinde güncelle
    camera.UpdateFrustum(projectionMatrix);

    int chunksRendered = 0; // Test için kaç chunk çizildiğini sayalım
    
    // Yüklü olan tüm chunkların üzerinden geç
    for(auto& pair : loadedChunks) {
        
        std::shared_ptr<Chunk> chunk = pair.second;
        
        // Eğer chunk ekran kartına yüklenmişse ve aktifse
        if (chunk->state == ACTIVE) {
            
            // 1. Chunk'ın dünyadaki minimum ve maksimum koordinatlarını bul
            // X ve Z koordinatları 16'nın katlarıdır. Yüksekliğimiz 256'dır.
            glm::vec3 chunkMin = glm::vec3(chunk->chunkX * 16.0f, 0.0f, chunk->chunkZ * 16.0f);
            glm::vec3 chunkMax = glm::vec3(chunkMin.x + 16.0f, 256.0f, chunkMin.z + 16.0f);

            // 2. CULLING TESTİ (Sihir burada gerçekleşiyor)
            if (camera.IsBoxVisible(chunkMin, chunkMax)) {
                
                // Chunk kameranın açısında! Ekran kartına çiz emri ver.
                glBindVertexArray(chunk->VAO);
                glDrawArrays(GL_TRIANGLES, 0, chunk->meshVertices.size() / 6); // (X,Y,Z,U,V,Shade = 6 parametre)
                
                chunksRendered++;
            }
        }
    }
    
    // std::cout << "Ekranda çizilen chunk sayısı: " << chunksRendered << std::endl;
}

```

Bu kodu çalıştırıp konsola (veya ekrana) `chunksRendered` değişkenini yazdırdığında göreceksin ki; eğer haritada 400 Chunk yüklüyse ve sen doğrudan yere ya da gökyüzüne bakıyorsan, sadece 10-20 Chunk çizilecektir. Arkanda kalan, kameranın görmediği 380 Chunk için ekran kartı (GPU) kılını bile kıpırdatmayacak, devasa bir CPU-GPU optimizasyonu sağlamış olacaksın.


# 16. soru

Tüm dünya ve optimizasyonlar hazır olduğuna göre, kameranın havada uçmasını iptal edip gerçek oyuncu fizikleri (yerçekimi, zıplama, bloklara çarpma/collision) sistemini nasıl kurabilirim?

# 16. cevap

Serbest kameradan çıkıp dünyaya fiziksel olarak ayak basmak, oyun motorunu bir "gözlemciden" gerçek bir "oyuna" dönüştüren en büyük adımdır.

Minecraft tarzı voxel oyunlarında, milyonlarca karmaşık üçgenle (Mesh) çarpışma hesaplamak yerine **AABB (Axis-Aligned Bounding Box - Eksenlere Hizalı Sınırlayıcı Kutu)** adı verilen çok daha basit ve yüksek performanslı bir fizik sistemi kullanılır.

İşte kamerayı uçan bir hayaletten yerçekimine tabi bir oyuncuya dönüştürmenin formülü:

## 1. Oyuncunun Fiziksel Sınırları (Hitbox)

Minecraft'ta oyuncunun genişliği 0.6 blok, yüksekliği ise 1.8 bloktur. Ayrıca havada uçmadığımız için artık bir "İvme" (Velocity) vektörüne ve yerçekimi kuvvetine ihtiyacımız var.

`Camera` veya `Player` sınıfına şu özellikleri eklemelisin:

```cpp
class Player {
public:
    glm::vec3 Position;
    glm::vec3 Velocity; // Karakterin anlık hareket yönü ve hızı
    
    // Karakterin fiziksel boyutu
    float width = 0.6f;
    float height = 1.8f;
    
    // Fizik kuralları
    float gravity = -20.0f;     // Saniyedeki düşüş hızı
    float jumpForce = 8.0f;     // Zıplama gücü
    float moveSpeed = 4.3f;     // Yürüme hızı
    
    bool isGrounded = false;    // Yere basıyor mu?
    
    // ... diğer kamera değişkenleri ...
};

```

---

## 2. Kutu Kesişim Testi (Collision Checker)

Karakterin bulunduğu konuma göre etrafındaki blokları bir kutu (Hitbox) içine alırız. Eğer bu kutunun içine giren koordinatlardan herhangi birinde katı bir blok (`blockID != 0`) varsa, karakter bir şeye çarpıyor demektir.

```cpp
// Verilen konumda karakterin kutusu herhangi bir bloğa çarpıyor mu?
bool IsColliding(glm::vec3 pos, World& world) {
    // Kutunun dünyadaki minimum ve maksimum X, Y, Z sınırlarını hesapla
    int minX = std::floor(pos.x - (width / 2.0f));
    int maxX = std::floor(pos.x + (width / 2.0f));
    
    int minY = std::floor(pos.y);                 // Ayaklar
    int maxY = std::floor(pos.y + height);        // Kafa
    
    int minZ = std::floor(pos.z - (width / 2.0f));
    int maxZ = std::floor(pos.z + (width / 2.0f));

    // Bu sınırlar içindeki TÜM tam sayı (Voxel) koordinatlarını tara
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            for (int z = minZ; z <= maxZ; z++) {
                
                // Eğer burada hava (0) dışında bir blok varsa çarpışma vardır!
                if (world.GetBlock(x, y, z) != 0) {
                    return true; 
                }
                
            }
        }
    }
    return false; // Kutunun içi tamamen hava
}

```

---

## 3. Duvarlarda Kayabilmek: Eksen Bazlı Hareket

Fizik motoru yazarken yapılan en büyük hata, X, Y ve Z eksenlerini aynı anda hareket ettirmektir. Eğer çapraz yürürken bir duvara çarparsan karakter tamamen durur ve duvara yapışır.

Bunun yerine her ekseni **tek tek** hareket ettirip kontrol etmeliyiz. Böylece X ekseninde duvara çarpsan bile Z ekseninde kayarak (slide) yürümeye devam edebilirsin.

```cpp
void Move(glm::vec3 deltaMove, World& world) {
    
    // 1. X EKSENİNDE HAREKET
    Position.x += deltaMove.x;
    if (IsColliding(Position, world)) {
        Position.x -= deltaMove.x; // Duvara çarptık, X'i geri al!
        Velocity.x = 0;            // X hızını sıfırla
    }

    // 2. Y EKSENİNDE HAREKET (Yerçekimi ve Zıplama)
    Position.y += deltaMove.y;
    if (IsColliding(Position, world)) {
        Position.y -= deltaMove.y; // Yere veya tavana çarptık, Y'yi geri al!
        
        // Eğer aşağı doğru düşerken çarptıysak yere basmışızdır
        if (deltaMove.y < 0) {
            isGrounded = true;
        }
        Velocity.y = 0; // Düşmeyi durdur
    } else {
        isGrounded = false; // Ayaklarımız yerden kesildi
    }

    // 3. Z EKSENİNDE HAREKET
    Position.z += deltaMove.z;
    if (IsColliding(Position, world)) {
        Position.z -= deltaMove.z; // Duvara çarptık, Z'yi geri al!
        Velocity.z = 0;
    }
}

```

---

## 4. Oyun Döngüsüne Fiziği Entegre Etmek

Artık uçan kamerayı iptal edip, klavye girdilerini İvme'ye (Velocity) dönüştürmelisin. `main.cpp` içindeki döngüde (`deltaTime`, yani bir önceki kareden bu yana geçen saniye ile çarparak) yerçekimini ve yürümeyi uygularız:

```cpp
// Her karede çalışacak Fizik Güncellemesi (Update)
void UpdateFizik(float deltaTime, World& world) {
    
    // 1. YERÇEKİMİ UYGULA (Sürekli aşağı doğru ivmelenme)
    Velocity.y += gravity * deltaTime;

    // 2. KLAVYE GİRDİLERİ (WASD)
    glm::vec3 walkDirection(0.0f);
    
    // Kamera yönüne (Front) sadece X ve Z ekseninde yürü (Gökyüzüne doğru yürümemek için Y'yi iptal et)
    glm::vec3 forward = glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, camera.Up));

    if (keyboard.W) walkDirection += forward;
    if (keyboard.S) walkDirection -= forward;
    if (keyboard.D) walkDirection += right;
    if (keyboard.A) walkDirection -= right;

    // Yürüyüş vektörünü normalize et ki çapraz koşarken hızlanmasın
    if (glm::length(walkDirection) > 0.1f) {
        walkDirection = glm::normalize(walkDirection);
    }
    
    // X ve Z hızını uygula
    Velocity.x = walkDirection.x * moveSpeed;
    Velocity.z = walkDirection.z * moveSpeed;

    // 3. ZIPLAMA (Space tuşu)
    if (keyboard.Space && isGrounded) {
        Velocity.y = jumpForce;
        isGrounded = false;
    }

    // 4. HAREKETİ GERÇEKLEŞTİR (Çarpışma kontrolleri ile birlikte)
    glm::vec3 frameMovement = Velocity * deltaTime;
    Move(frameMovement, world);
}

```

Bu kodları eklediğinde, blokların üzerinde yürüyebildiğini, Space tuşuyla zıplayabildiğini ve dağların zirvesinden atladığında yerçekimi ivmesiyle hızlanarak vadinin tabanına gerçekçi bir şekilde çarptığını hissedeceksin!


# 17. soru

evet ekleyelim hatta daha fazla blok ekleyip ağaç yapısı da eklemek istiyorum, sadece canlı olmasın. 
eklemek istediğim bloklar:
Stone
Granite
Diorite
Andesite
Grass
Dirt
Cobblestone
Oak Wood
Oak Wood Plank
Oak Sapling
Apple
Bedrock
Sand
Oak Leaves
Bu yukarıdaki item veya bloklarla beraber bir biyom oluşturma ve o biyom üzerinde ağaç oluşturma işine de girelim ayrıca tek bir biyom değil birden çok biyom olsun istiyorum.
ekleyeceğim şeyler:
Cactus
Dead Shrub
Dead Bush
Grass
Fern

ekleyeceğim ilk canlı tavuk olucak,
onunla ile ilgili olarak:
Raw Chicken
Cooked Chicken
Feather
egg

oyuna ekleyeceğim ilk düşman zombi olucak.
onunla ile ilgili olarak:
zombiyi öldürdüğümüzde
Rotten Flesh düşecek.



# 17. cevap

Oyun motorunu bir "teknoloji demosundan" çıkarıp gerçekten yaşayan, oynanabilir bir evrene dönüştürecek o büyük adıma geldik. İstediğin bloklar, bitki örtüleri, biyomlar ve canlılar oyuna muazzam bir derinlik katacak.

Ancak bu kadar fazla içerik eklendiğinde, kodları rastgele yazmaya devam edemeyiz. Önce sistemimizi bu devasa veriyi yönetecek şekilde kategorize etmeliyiz.

## 1. Blok ve Eşya Kayıt Sistemi (Registry)

Oyunda artık sadece yerleştirilebilir "Bloklar" değil, aynı zamanda sadece envanterde durabilen "Eşyalar" (Elma, Tüy, Çiğ Tavuk vb.) da var. Bunları sayılarla (1, 2, 3) ezberlemek yerine C++ `enum` yapısını kullanarak motoruna bir kimlik kütüphanesi kazandırmalısın.

```cpp
// include/Registry.h
enum ItemID {
    // 0 - 255 Arası: Dünyaya yerleştirilebilen Bloklar
    AIR = 0, STONE = 1, GRANITE, DIORITE, ANDESITE, GRASS_BLOCK, DIRT, 
    COBBLESTONE, OAK_LOG, OAK_PLANKS, OAK_SAPLING, BEDROCK, SAND, 
    OAK_LEAVES, CACTUS, DEAD_BUSH, FERN,
    
    // 256 ve Üzeri: Sadece envanterde olan Eşyalar (Items)
    APPLE = 256, RAW_CHICKEN, COOKED_CHICKEN, FEATHER, EGG, ROTTEN_FLESH
};

```

*Not: Zombi öldüğünde düşen `ROTTEN_FLESH` (Çürük Et) veya `APPLE` (Elma) için oyuncu sağ tıkladığında dünyada bir blok oluşmaz, bu ayrımı `ID < 256` kontrolüyle çok kolay yapabilirsin.*

---

## 2. Çoklu Biyom ve Ağaç Üretimi (World Generation)

Farklı biyomlar (Örn: Çöl ve Orman) oluşturmak için tek bir Perlin Noise haritası yetmez. **İki farklı gürültü haritası** kullanmalıyız:

1. **Yükseklik Haritası (HeightMap):** Dağları ve vadileri belirler.
2. **Sıcaklık Haritası (TemperatureMap):** O bölgenin Çöl mü yoksa Orman mı olacağını belirler.

`Chunk` sınıfındaki arazi üretim fonksiyonunu şu şekilde güncellemeliyiz:

```cpp
void GenerateTerrain() {
    FastNoiseLite heightNoise, tempNoise;
    heightNoise.SetFrequency(0.01f);
    tempNoise.SetFrequency(0.005f); // Sıcaklık daha yavaş değişir (Biyomlar geniş olur)

    for(int x = 0; x < 16; x++) {
        for(int z = 0; z < 16; z++) {
            float globalX = (chunkX * 16.0f) + x;
            float globalZ = (chunkZ * 16.0f) + z;

            int terrainHeight = 64 + (int)(heightNoise.GetNoise(globalX, globalZ) * 30.0f);
            float temperature = tempNoise.GetNoise(globalX, globalZ); // -1.0 ile 1.0 arası

            // --- BİYOM SEÇİMİ ---
            bool isDesert = (temperature > 0.3f); // Sıcaklık yüksekse Çöl Biyomu

            for(int y = 0; y < 256; y++) {
                if (y == 0) {
                    blocks[x][y][z] = BEDROCK; // En alt katman kırılmaz kaya
                } 
                else if (y == terrainHeight) {
                    // Yüzey Bloğu
                    blocks[x][y][z] = isDesert ? SAND : GRASS_BLOCK;
                } 
                else if (y < terrainHeight && y > terrainHeight - 4) {
                    // Yüzeyin Altı
                    blocks[x][y][z] = isDesert ? SAND : DIRT;
                } 
                else if (y <= terrainHeight - 4) {
                    // Derinler (Rastgele Madenler eklenebilir)
                    int randStone = rand() % 100;
                    if (randStone < 5) blocks[x][y][z] = GRANITE;
                    else if (randStone < 10) blocks[x][y][z] = DIORITE;
                    else if (randStone < 15) blocks[x][y][z] = ANDESITE;
                    else blocks[x][y][z] = STONE;
                }
            }

            // --- DEKORASYON (Ağaçlar ve Bitkiler) ---
            // Sadece yüzeyin üzerinde ve rastgele noktalarda bitki oluştur
            int randPlant = rand() % 100;
            int surfaceY = terrainHeight + 1;

            if (isDesert) {
                if (randPlant < 2) {
                    // Kaktüs (2-3 blok uzunluğunda)
                    int cactusHeight = (rand() % 2) + 2;
                    for(int h = 0; h < cactusHeight; h++) blocks[x][surfaceY + h][z] = CACTUS;
                } else if (randPlant < 5) {
                    blocks[x][surfaceY][z] = DEAD_BUSH; // Ölü çalı
                }
            } 
            else { // Orman Biyomu
                if (randPlant < 1) {
                    // AĞAÇ OLUŞTURMA (Basit Meşe Ağacı)
                    // Not: X ve Z'nin chunk sınırlarını aşmaması için 2 ile 13 arası sınır koyuyoruz
                    if (x > 2 && x < 13 && z > 2 && z < 13) {
                        int trunkHeight = 4 + (rand() % 2); // 4-5 blok gövde
                        
                        // 1. Gövdeyi (Log) dik
                        for (int h = 0; h < trunkHeight; h++) {
                            blocks[x][surfaceY + h][z] = OAK_LOG;
                        }
                        
                        // 2. Yaprakları (Leaves) gövdenin tepesine küre/kutu şeklinde ekle
                        for (int lx = -2; lx <= 2; lx++) {
                            for (int ly = 0; ly <= 2; ly++) {
                                for (int lz = -2; lz <= 2; lz++) {
                                    // Ağacın köşelerini kırmak için basit bir kontrol
                                    if (abs(lx) == 2 && abs(lz) == 2 && ly == 2) continue; 
                                    
                                    // Eğer o koordinat boşsa yaprak koy
                                    if (blocks[x + lx][surfaceY + trunkHeight - 1 + ly][z + lz] == AIR) {
                                        blocks[x + lx][surfaceY + trunkHeight - 1 + ly][z + lz] = OAK_LEAVES;
                                    }
                                }
                            }
                        }
                    }
                } else if (randPlant < 15) {
                    blocks[x][surfaceY][z] = (rand() % 2 == 0) ? GRASS : FERN;
                }
            }
        }
    }
}

```

---

## 3. Canlılar ve Düşmanlar (Entity System)

Zombiler ve Tavuklar için nesne yönelimli (OOP) bir temel oluşturmalıyız. Her canlının bir pozisyonu, canı ve öldüğünde düşüreceği eşyalar olmalıdır.

```cpp
class Entity {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    float health;
    bool isDead = false;

    // Sanal (Virtual) fonksiyonlar ile her canlının yapay zekasını ayırıyoruz
    virtual void UpdateAI(float deltaTime, glm::vec3 playerPos) = 0;
    virtual void OnDeath(Player& player) = 0; 
};

```

### Tavuk Sınıfı (Pasif / Kaçan)

Tavuklar etrafta rastgele gezinir ve belirli aralıklarla yumurtlarlar.

```cpp
class Chicken : public Entity {
public:
    float eggTimer = 300.0f; // Saniye cinsinden yumurtlama süresi

    Chicken(glm::vec3 startPos) {
        position = startPos;
        health = 4.0f; // 2 Kalp
    }

    void UpdateAI(float deltaTime, glm::vec3 playerPos) override {
        // Rastgele gezinme matematiği (Yönü rastgele değiştir ve ilerle)
        
        // Yumurtlama Mekaniği
        eggTimer -= deltaTime;
        if (eggTimer <= 0.0f) {
            // Bulunduğu konuma EGG (Yumurta) eşyası bırak
            // world.SpawnItem(position, EGG);
            eggTimer = 300.0f; // Süreyi sıfırla
        }
    }

    void OnDeath(Player& player) override {
        // Tavuk öldüğünde %100 Çiğ Tavuk, %50 ihtimalle Tüy düşürür
        player.GiveItem(RAW_CHICKEN, 1);
        if (rand() % 100 < 50) {
            player.GiveItem(FEATHER, 1);
        }
    }
};

```

### Zombi Sınıfı (Agresif / Takip Eden)

Zombilerin tek amacı oyuncunun koordinatına doğru hareket etmektir (Pathfinding/Yönelme).

```cpp
class Zombie : public Entity {
public:
    float speed = 2.0f;

    Zombie(glm::vec3 startPos) {
        position = startPos;
        health = 20.0f; // 10 Kalp
    }

    void UpdateAI(float deltaTime, glm::vec3 playerPos) override {
        // Oyuncu ile Zombi arasındaki mesafeyi ölç
        glm::vec3 direction = playerPos - position;
        float distance = glm::length(direction);

        // Eğer oyuncu görüş alanındaysa (Örn: 16 blok) üzerine yürü
        if (distance < 16.0f && distance > 0.8f) {
            glm::vec3 walkDir = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
            velocity.x = walkDir.x * speed;
            velocity.z = walkDir.z * speed;
        } else {
            velocity.x = 0; velocity.z = 0; // Dur
        }
    }

    void OnDeath(Player& player) override {
        // Zombi öldüğünde 1-2 adet Çürük Et düşürür
        int fleshCount = (rand() % 2) + 1;
        player.GiveItem(ROTTEN_FLESH, fleshCount);
    }
};

```

Ana oyun döngünde bu varlıkları bir liste (`std::vector<Entity*> activeEntities`) içinde tutup her karede `UpdateAI` fonksiyonlarını çağırdığında; tavukların çimenlerde dolandığını, zombilerin ise seni gördüğü an üzerine doğru yürümeye başladığını göreceksin!


# 18. Soru

Zombilerin doğrudan üzerime gelmesi (Pathfinding) ve oyuncuya kılıçla/elle vurup hasar verebilmesi için gerekli olan saldırı (Combat) sistemini nasıl kurabilirim?

# 18. Cevap

Oyun motoruna savaş (Combat) ve hasar mekaniklerini eklemek, projenin en heyecanlı aşamalarından biridir. Ancak burada grafik veya fizikten çok **Oyun Tasarımı Matematiği** devreye girer.

Eğer bir zombi oyuncuya değdiği an doğrudan canını azaltırsan, oyun saniyede 60 kare (60 FPS) çalıştığı için zombi oyuncuya saniyede 60 kez hasar verir ve oyuncu saliseler içinde ölür. Minecraft'ta bir canlının hasar aldıktan sonra kısa bir süreliğine kırmızıya dönüp hasar almaz hale gelmesinin sebebi budur. Buna **I-Frames (Invulnerability Frames - Hasar Alınmazlık Süresi)** denir.

İşte tam teşekküllü bir savaş sistemini motoruna entegre etmenin adımları:

### 1. I-Frames ve Hasar Alma Sistemi (Player ve Entity)

Hem oyuncunun hem de canlıların (Tavuk, Zombi) hasar alabilmesi için temel sınıflarımıza bir "Dokunulmazlık Sayacı" ve hasar alma fonksiyonu eklemeliyiz.

`Entity` sınıfına (ve `Player` sınıfına benzer şekilde) şu özellikleri ekle:

```cpp
class Entity {
public:
    // ... önceki değişkenler (position, velocity, health) ...
    
    float width = 0.6f;
    float height = 1.8f;
    
    float invulnerabilityTimer = 0.0f; // Hasar alınmazlık süresi
    
    // Hasar alma ve Geri Tepme (Knockback) fonksiyonu
    virtual void TakeDamage(float amount, glm::vec3 attackerPos) {
        if (invulnerabilityTimer > 0.0f) return; // Zaten hasar almışsa bekle

        health -= amount;
        invulnerabilityTimer = 0.5f; // Yarım saniye boyunca tekrar hasar alamaz

        if (health <= 0.0f) {
            isDead = true;
            // OnDeath(player) fonksiyonu burada tetiklenebilir
        } else {
            // GERİ TEPME (Knockback) FİZİĞİ
            // Saldırgandan bu canlıya doğru olan yönü bul
            glm::vec3 knockbackDir = glm::normalize(position - attackerPos);
            knockbackDir.y = 0.5f; // Hafifçe havaya zıplat
            
            velocity = knockbackDir * 5.0f; // Geriye doğru fırlat
        }
    }
    
    // Hitbox (Sınırlayıcı Kutu) sınırlarını döndüren yardımcı fonksiyon
    bool IsPointInside(glm::vec3 point) {
        return (point.x >= position.x - width/2 && point.x <= position.x + width/2 &&
                point.y >= position.y && point.y <= position.y + height &&
                point.z >= position.z - width/2 && point.z <= position.z + width/2);
    }
};

```

---

### 2. Zombi Yapay Zekası: Takip ve Saldırı

Zombinin `UpdateAI` fonksiyonunu sadece yürümekle kalmayıp, oyuncuya yeterince yaklaştığında (Örn: 1.5 blok) hasar verecek şekilde güncellemeliyiz.

```cpp
class Zombie : public Entity {
public:
    float speed = 2.5f;
    float attackCooldown = 0.0f;
    float attackDamage = 2.0f; // 1 Tam Kalp (Her kalp 2 birimdir)

    Zombie(glm::vec3 startPos) {
        position = startPos;
        health = 20.0f; 
    }

    // playerPos yerine doğrudan Player referansı alıyoruz ki canını azaltabilelim
    void UpdateAI(float deltaTime, Player& player) override {
        // I-Frame ve Saldırı sürelerini güncelle
        if (invulnerabilityTimer > 0.0f) invulnerabilityTimer -= deltaTime;
        if (attackCooldown > 0.0f) attackCooldown -= deltaTime;

        glm::vec3 direction = player.Position - position;
        float distance = glm::length(direction);

        if (distance < 16.0f) { // Görüş Menzili
            
            if (distance <= 1.5f) { 
                // SALDIRI MENZİLİNDE!
                velocity.x = 0; velocity.z = 0; // Durarak vur
                
                if (attackCooldown <= 0.0f) {
                    player.TakeDamage(attackDamage, this->position);
                    attackCooldown = 1.0f; // Saniyede 1 kez vurabilir
                }
            } 
            else {
                // TAKİP ET (Sadece X ve Z ekseninde yürü, havaya uçma)
                glm::vec3 walkDir = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
                
                // Eğer knockback yiyorsa (havadaysa) yürüme hızını kısıtlayabilirsin
                velocity.x = walkDir.x * speed;
                velocity.z = walkDir.z * speed;
            }
        }
        
        // Zombinin fizikleri de tıpkı oyuncu gibi Move() fonksiyonu ile güncellenmeli (Yerçekimi dahil)
    }
};

```

---

### 3. Oyuncunun Kılıçla / Elle Vurması (Entity Raycasting)

Önceki adımlarda blok kırmak için eklediğimiz `InteractWithBlock` fonksiyonunda bir ışın (Ray) yolluyorduk. Şimdi bu ışının ilerlediği her adımda **"Bu nokta bir yaratığın Hitbox'ının içinde mi?"** diye kontrol etmeliyiz. Eğer içindeyse, bloğa çarpmadan önce yaratığa çarpıp hasar vermeliyiz.

Ana dosyanızdaki (main.cpp) sol tık etkileşimini şu şekilde güncelle:

```cpp
void PerformLeftClick(Player& player, World& world, std::vector<Entity*>& activeEntities) {
    glm::vec3 rayPosition = player.Position + glm::vec3(0.0f, 1.6f, 0.0f); // Göz hizasından başla
    glm::vec3 rayDirection = camera.Front;
    
    float maxReach = 4.0f; // Oyuncunun vurma/kırma menzili
    float stepSize = 0.1f;

    for (float t = 0; t < maxReach; t += stepSize) {
        glm::vec3 currentPoint = rayPosition + (rayDirection * t);

        // 1. ÖNCE CANLILARA (ENTITY) ÇARPIŞMA KONTROLÜ
        bool hitEntity = false;
        for (Entity* entity : activeEntities) {
            if (!entity->isDead && entity->IsPointInside(currentPoint)) {
                
                // Işın Zombiye veya Tavuğa çarptı! Hasar ver.
                // Eğer oyuncunun elinde (Active Slot) kılıç varsa hasarı artırabilirsin
                float damage = 2.0f; // Yumruk hasarı = 1 kalp
                if (player.GetActiveItem() == STONE_SWORD) damage = 5.0f; 

                entity->TakeDamage(damage, player.Position);
                hitEntity = true;
                break; // Aynı anda iki yaratığa vurmamak için döngüden çık
            }
        }
        
        if (hitEntity) return; // Yaratığa vurduysak arkasındaki bloğu kırma, işlemi bitir!

        // 2. EĞER CANLI YOKSA BLOK KIRMA KONTROLÜ
        int blockX = std::floor(currentPoint.x);
        int blockY = std::floor(currentPoint.y);
        int blockZ = std::floor(currentPoint.z);

        if (world.GetBlock(blockX, blockY, blockZ) != AIR) {
            // Bloğu Kır
            world.SetBlock(blockX, blockY, blockZ, AIR);
            // Eşyayı envantere ekleme kodları...
            break; 
        }
    }
}

```

---

### 4. Vuruş Hissiyatı (Görsel Geri Bildirim)

Savaş sistemlerinin ruhsuz hissettirmesinin sebebi genellikle geri bildirimin (Feedback) eksikliğidir. Mükemmel bir vuruş hissiyatı için şu iki detayı Shader'ına ekleyebilirsin:

Zombiyi (veya diğer yaratıkları) 3D olarak ekrana çizerken, eğer `invulnerabilityTimer > 0.0f` ise, `fragment.glsl` dosyasında yaratığın dokusunun üzerine kırmızı bir ton bindirmelisin:

```glsl
// Yaratık çizimi için Fragment Shader eklentisi
uniform bool isTakingDamage;

void main() {
    vec4 texColor = texture(ourTexture, TexCoord);
    
    if (isTakingDamage) {
        // Rengi kırmızıya kaydır
        texColor = mix(texColor, vec4(1.0, 0.0, 0.0, texColor.a), 0.5); 
    }
    
    FragColor = texColor;
}

```

C++ tarafında yaratığı çizerken:

```cpp
shader.setBool("isTakingDamage", entity->invulnerabilityTimer > 0.0f);

```

Bu adımlarla beraber zombilerin seni gördüğünde üzerine koştuğu, vurduğunda kırmızıya dönerek geriye sektiği (Knockback) ve canları bittiğinde Çürük Et (Rotten Flesh) düşürerek öldüğü tam işlevsel bir hayatta kalma savaş mekaniğine sahip oluyorsun!

# 19. Soru

Fizik ve savaş mekanikleri çalışıyor. Tavukları ve Zombileri sadece hareket eden kutular olmaktan çıkarıp, onlara Minecraft'taki gibi bacakları ve kolları olan 3D bir model (Küp hiyerarşisi) ve yürüme animasyonu nasıl ekleyebilirim?

# 19. Cevap

Bir oyun motorunda canlıları (Entity) tek bir düz kutu olmaktan çıkarıp, kolları ve bacakları hareket eden karakterlere dönüştürmek "Skelet Model (Skeletal Animation)" veya "Hiyerarşik Küp Sistemi" ile yapılır.

Minecraft tarzı oyunlarda karakterlerin kolları ve bacakları dirseklerden bükülmez, sadece omuzdan veya kalçadan tek parça halinde sallanır. Bu yüzden profesyonel, kemikli 3D animasyon (Rigging) sistemlerine ihtiyacımız yok. Zombi dediğimiz şey aslında **6 adet ayrı küpün** (Kafa, Gövde, Sağ Kol, Sol Kol, Sağ Bacak, Sol Bacak) matematiksel olarak birbirine bağlanmasıdır.

Burada çözmemiz gereken en büyük sorun **Pivot (Eklem) Noktalarıdır**. Eğer bir küpü kendi merkezinden (tam ortasından) döndürürsen, zombinin kolu bir pervane gibi döner. Bizim onu omuz noktasından döndürmemiz gerekir.

İşte bu sistemi C++ ile motoruna adım adım eklemenin yolu:

## 1. Model Parçası (Model Part) Sınıfını Kurmak

Her bir uzuv (Kafa, Kol vb.) kendi boyutuna, eklem noktasına ve anlık dönüş açısına sahip olmalıdır. `include/` klasörüne `ModelPart` adında yeni bir yapı oluştur:

```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct ModelPart {
    glm::vec3 size;          // Parçanın büyüklüğü (Örn: Kol için 0.25 x 0.75 x 0.25)
    glm::vec3 pivot;         // Döndürüleceği eklem noktası (Göbeğe göre koordinatı)
    glm::vec3 rotation;      // Anlık dönüş açıları (X, Y, Z - Radyan cinsinden)
    
    // Çizim sırasında bu parçanın yerini ve dönüşünü hesaplayan fonksiyon
    glm::mat4 GetTransformMatrix(glm::vec3 entityPosition, float entityYaw) {
        glm::mat4 model = glm::mat4(1.0f);
        
        // 1. Parçayı dünyada karakterin olduğu yere taşı
        model = glm::translate(model, entityPosition);
        
        // 2. Karakterin baktığı yöne (Yaw) göre tüm modeli döndür
        model = glm::rotate(model, entityYaw, glm::vec3(0, 1, 0));
        
        // 3. Eklem noktasına (Pivot) git
        model = glm::translate(model, pivot);
        
        // 4. Parçayı eklem yerinden döndür (Animasyon burada çalışır)
        model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0)); // İleri/Geri sallanma
        model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0)); // Sağa/Sola bakma
        model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1)); // Yana yatma
        
        // 5. Küpün merkezini pivot noktasına göre hizalamak için aşağı kaydır
        // (Çünkü OpenGL'de küpler merkezden çizilir, omuzdan değil)
        model = glm::translate(model, glm::vec3(0, -size.y / 2.0f, 0));
        
        // 6. Küpü kendi boyutlarına göre sündür (Scale)
        model = glm::scale(model, size);
        
        return model;
    }
};

```

---

## 2. Zombi Modelini Tanımlamak

Şimdi `Zombie` sınıfımızın içine bu 6 parçayı yerleştirip boyutlarını Minecraft standartlarına (1 blok = 1.0 birim) göre ayarlayalım:

```cpp
class Zombie : public Entity {
public:
    ModelPart head, body, leftArm, rightArm, leftLeg, rightLeg;
    
    // Karakterin dünyada baktığı yön açısı (Radyan)
    float yaw = 0.0f; 
    
    // Animasyon için geçici zaman tutucu
    float animationTime = 0.0f;

    Zombie(glm::vec3 startPos) {
        position = startPos;
        health = 20.0f;

        // --- ZOMBİ ANATOMİSİ ---
        // (Not: Y ekseninde 0 noktası Zombinin göbeğidir)

        // Gövde (Body)
        body.size = glm::vec3(0.5f, 0.75f, 0.25f);
        body.pivot = glm::vec3(0.0f, 0.375f, 0.0f);

        // Kafa (Head)
        head.size = glm::vec3(0.5f, 0.5f, 0.5f);
        head.pivot = glm::vec3(0.0f, 0.75f, 0.0f); // Boyun noktası

        // Kollar (Omuzlardan bağlanır)
        rightArm.size = glm::vec3(0.25f, 0.75f, 0.25f);
        rightArm.pivot = glm::vec3(-0.375f, 0.75f, 0.0f); // Sol omuz
        
        leftArm.size = glm::vec3(0.25f, 0.75f, 0.25f);
        leftArm.pivot = glm::vec3(0.375f, 0.75f, 0.0f); // Sağ omuz

        // Bacaklar (Kalçadan bağlanır)
        rightLeg.size = glm::vec3(0.25f, 0.75f, 0.25f);
        rightLeg.pivot = glm::vec3(-0.125f, 0.0f, 0.0f); // Sol kalça

        leftLeg.size = glm::vec3(0.25f, 0.75f, 0.25f);
        leftLeg.pivot = glm::vec3(0.125f, 0.0f, 0.0f); // Sağ kalça
    }
    
    // ... UpdateAI ve OnDeath fonksiyonları aynen kalacak ...
};

```

---

## 3. Yürüme Animasyonu (Kinematik)

Canlıların animasyonunu hesaplamak için karmaşık dosyalara ihtiyacımız yok. Kolların ve bacakların ileri geri sallanması doğada sinüs (Sine) dalgası şeklinde gerçekleşir. Sağ kol ileri giderken sol bacak ileri gider; tam tersi durumda da zıt yönde hareket ederler.

`Zombie` sınıfının içindeki `UpdateAI` fonksiyonuna animasyon matematiğini ekleyelim:

```cpp
void UpdateAI(float deltaTime, Player& player) override {
    // ... Mevcut takip ve hasar kodları burada çalışır ...

    // Zombinin yönünü (Yaw) oyuncuya bakacak şekilde güncelle
    glm::vec3 direction = player.Position - position;
    yaw = atan2(direction.x, direction.z); // Zombiyi oyuncuya döndürür

    // Hızına göre animasyon zamanını artır (Eğer duruyorsa hız 0'dır, sallanma durur)
    float currentSpeed = glm::length(glm::vec2(velocity.x, velocity.z));
    
    if (currentSpeed > 0.1f) {
        animationTime += deltaTime * 10.0f; // Sallanma hızı
    } else {
        // Duruyorsa kolları ve bacakları yavaşça normal pozisyona (0) geri getir
        animationTime = 0.0f; 
    }

    // Sinüs dalgası ile sallanma açısını hesapla (Maksimum 60 derece / 1.0 radyan)
    float swingAngle = sin(animationTime) * 1.0f;

    // Kollar ve Bacaklar ZIT yönlerde çalışır
    rightArm.rotation.x = swingAngle;
    leftArm.rotation.x = -swingAngle;
    
    rightLeg.rotation.x = -swingAngle;
    leftLeg.rotation.x = swingAngle;

    // Zombilerin kolları her zaman biraz öne doğru kalkıktır (Minecraft klasiği)
    rightArm.rotation.x -= 1.5f; // ~90 Derece öne kaldır
    leftArm.rotation.x -= 1.5f;
    
    // Kafa hafifçe sallanabilir veya oyuncuya bakabilir
    // head.rotation.y = ... 
}

```

---

## 4. 3D Çizim (Render) Aşaması

Dünyayı çizerken, zombinin (veya tavuğun) üzerindeki bu parçaları sırayla çizmemiz gerekir. Bunun için 3D UI kodlarında kullandığımız gibi 1x1x1 boyutlarında, merkezi `(0,0,0)` olan **tek bir standart Küp VAO'su (Vertex Array Object)** oluşturmalısın.

Sonra zombiyi çizerken şu adımları izlersin:

```cpp
void RenderZombie(Zombie& zombie, unsigned int zombieTexture, unsigned int cubeVAO, Shader& shader3D) {
    
    // 1. Zombi dokusunu (zombie.png) aktif et
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, zombieTexture);
    glBindVertexArray(cubeVAO);

    // Kırmızı hasar efektini uygula (Önceki adımdan)
    shader3D.setBool("isTakingDamage", zombie.invulnerabilityTimer > 0.0f);

    // 2. Her parçanın Transform matrisini hesapla ve tek tek çiz
    
    // KAFA
    shader3D.setMat4("model", zombie.head.GetTransformMatrix(zombie.position, zombie.yaw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // GÖVDE
    shader3D.setMat4("model", zombie.body.GetTransformMatrix(zombie.position, zombie.yaw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // KOLLAR
    shader3D.setMat4("model", zombie.leftArm.GetTransformMatrix(zombie.position, zombie.yaw));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    shader3D.setMat4("model", zombie.rightArm.GetTransformMatrix(zombie.position, zombie.yaw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // BACAKLAR
    shader3D.setMat4("model", zombie.leftLeg.GetTransformMatrix(zombie.position, zombie.yaw));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    shader3D.setMat4("model", zombie.rightLeg.GetTransformMatrix(zombie.position, zombie.yaw));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

```

> **Not:** Eğer parçalara zombi dokusunu doğru giydirmek (UV Mapping) istersen, 1x1 küpünü tek bir sabit UV ile değil; aynen dünya bloklarında yaptığımız gibi Shader'a `uvOffset` (Yüz, gövde, kıyafet koordinatları) göndererek çizmeli veya Kafa, Gövde, Kol için UV'leri önceden kesilmiş ayrı VAO'lar kullanmalısın.

Aynı `ModelPart` mantığıyla `Chicken` sınıfına gövde, kafa, gaga, sağ kanat, sol kanat ve iki bacak tanımlayıp; `UpdateAI` içinde kollar yerine kanatları çırptırabilirsin.

# 20. soru

Grafiklerimiz, fizik motorumuz ve canlılarımız kusursuz çalışıyor. Peki, yaptığım bu devasa dünyayı kapatıp açtığımda kaybolmaması için Chunk'ları ve Envanteri bir dosyaya nasıl kaydedip (Save) geri yüklerim (Load)?

# 20. Cevap

Oyun motorunu yaparken geldiğin bu nokta, artık projenin bir oyuna dönüştüğü eşiktir. Devasa dünyaları ve envanteri kaydetmek için JSON veya XML gibi metin tabanlı (Text) formatlar **kullanılamaz**. Sadece tek bir Chunk içinde 65.536 blok vardır; bunu metin olarak kaydetmek hem diski saniyeler içinde doldurur hem de oyunun donmasına yol açar.

Çözüm: **Binary (İkili) Kayıt Sistemi**. Bellekteki veriyi (0 ve 1'leri) olduğu gibi alıp doğrudan diske yazacağız. Bu işlem milisaniyeler sürer ve çok az yer kaplar.

İşte motoruna profesyonel bir Save/Load (Kaydet/Yükle) sistemi kurmanın adımları:

## 1. Klasör Yapısı

Öncelikle oyunun ana dizininde kayıt dosyalarını tutacak bir klasör yapısı olmalıdır. C++17 ile gelen `<filesystem>` kütüphanesini kullanarak bu klasörleri otomatik oluşturabiliriz.

```text
saves/
└── Dunyam1/
    ├── player.dat       (Oyuncu konumu, canı ve envanteri)
    └── chunks/
        ├── 0_0.chunk    (0,0 koordinatındaki chunk)
        ├── 1_0.chunk
        └── -1_2.chunk

```

---

## 2. Oyuncu ve Envanteri Kaydetmek

`Player` sınıfındaki verilerimizi sabit boyutlu bir yapıda kurguladığımız için (örneğin tam 36 slotluk envanter) bunları diske tek bir hamlede yazabiliriz.

```cpp
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void SavePlayer(Player& player, std::string worldName) {
    // Klasör yoksa oluştur
    std::string path = "saves/" + worldName + "/";
    fs::create_directories(path);

    // Dosyayı İkili (Binary) formatta yazmak üzere aç
    std::ofstream file(path + "player.dat", std::ios::binary);
    
    if (file.is_open()) {
        // 1. Oyuncu Konumunu Yaz (3 adet float)
        file.write(reinterpret_cast<char*>(&player.Position), sizeof(glm::vec3));
        
        // 2. Oyuncu Yönünü (Kamera Açıları) Yaz
        file.write(reinterpret_cast<char*>(&camera.Yaw), sizeof(float));
        file.write(reinterpret_cast<char*>(&camera.Pitch), sizeof(float));
        
        // 3. Canı Yaz
        file.write(reinterpret_cast<char*>(&player.health), sizeof(float));
        
        // 4. Envanteri Yaz (std::vector'ün bellekteki ham verisini doğrudan yazıyoruz)
        file.write(reinterpret_cast<char*>(player.inventory.data()), player.inventory.size() * sizeof(InventorySlot));
        
        file.close();
    }
}

void LoadPlayer(Player& player, std::string worldName) {
    std::string path = "saves/" + worldName + "/player.dat";
    std::ifstream file(path, std::ios::binary);
    
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&player.Position), sizeof(glm::vec3));
        file.read(reinterpret_cast<char*>(&camera.Yaw), sizeof(float));
        file.read(reinterpret_cast<char*>(&camera.Pitch), sizeof(float));
        file.read(reinterpret_cast<char*>(&player.health), sizeof(float));
        
        // Envanteri oku
        file.read(reinterpret_cast<char*>(player.inventory.data()), player.inventory.size() * sizeof(InventorySlot));
        
        file.close();
    }
}

```

---

## 3. Chunk'ları (Dünyayı) Kaydetmek

Bir Chunk'ın içindeki blok dizisi (`uint8_t blocks[16][256][16]`) bellekte ardışık (contiguous) olarak durur. 65.536 byte boyutundaki bu veriyi diske saniyede yüzlerce kez yazabilirsin, performansı hiç etkilemez.

`Chunk` sınıfına şu fonksiyonları ekle:

```cpp
void SaveToFile(std::string worldName) {
    std::string path = "saves/" + worldName + "/chunks/";
    fs::create_directories(path);
    
    // Dosya adını koordinatlara göre belirle (Örn: 5_-2.chunk)
    std::string filename = path + std::to_string(chunkX) + "_" + std::to_string(chunkZ) + ".chunk";
    
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        // Tüm blok dizisini tek hamlede diske yapıştır
        file.write(reinterpret_cast<char*>(&blocks), sizeof(blocks));
        file.close();
    }
}

bool LoadFromFile(std::string worldName) {
    std::string filename = "saves/" + worldName + "/chunks/" + std::to_string(chunkX) + "_" + std::to_string(chunkZ) + ".chunk";
    
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        // Diskten oku ve blok dizisinin üzerine yaz
        file.read(reinterpret_cast<char*>(&blocks), sizeof(blocks));
        file.close();
        return true; // Başarıyla yüklendi
    }
    return false; // Dosya yok (Demek ki bu Chunk hiç oluşturulmamış)
}

```

---

## 4. Chunk Yöneticisini (World Sınıfı) Güncellemek

Dünya üretimini (Perlin Noise) yaptığımız kısıma çok küçük ama hayati bir mantık eklemeliyiz. Yeni bir Chunk üretmeden önce diske bakmalı; eğer dosya varsa Perlin Noise'u iptal edip dosyadan okumalıyız.

Daha önce `World::Update` içinde yazdığımız Chunk oluşturma kısmını şu şekilde değiştir:

```cpp
// Eğer bu chunk RAM'de yoksa, yeni oluştur!
if (loadedChunks.find(coord) == loadedChunks.end()) {
    
    auto newChunk = std::make_shared<Chunk>(x, z);
    loadedChunks[coord] = newChunk;

    std::thread([this, newChunk]() {
        
        // 1. Önce diski kontrol et!
        bool isLoaded = newChunk->LoadFromFile("Dunyam1");
        
        // 2. Eğer diskte yoksa, YENİDEN OLUŞTUR (Perlin Noise)
        if (!isLoaded) {
            newChunk->GenerateTerrain();
        }

        // 3. Her halükarda (ister dosyadan gelsin ister yeni üretilsin) dış yüzeyi hesapla
        newChunk->GenerateMesh();    

        // Kuyruğa ekle...
        {
            std::lock_guard<std::mutex> lock(this->queueMutex);
            this->chunksToBuffer.push(newChunk);
        }

    }).detach();
}

```

### Chunk Silinirken Kaydetmek

Oyuncu dünyada yürüdükçe, arkasında kalan ve Görüş Mesafesinden (Render Distance) çıkan Chunk'ları RAM'i temizlemek için siliyorduk (`erase()`). Silmeden hemen önce onları kaydetmelisin:

```cpp
// Chunk'ı RAM'den silmeden HEMEN ÖNCE:
chunk->SaveToFile("Dunyam1");
loadedChunks.erase(coord);

```

Ve oyun kapanırken (Oyun Döngüsü kırıldığında) `SavePlayer(player, "Dunyam1");` çağrısı yapıp tüm aktif Chunk'ları son bir kez kaydederek çıkış yaparsın.

> **İleri Seviye Optimizasyon Notu:** Oyunun boyutu zamanla büyüyecektir (Her chunk 65KB). İleride RLE (Run-Length Encoding) algoritmasını araştırarak bu kayıt dosyalarını %90 oranında küçültebilirsin (Örn: 3000 adet hava bloğunu tek tek kaydetmek yerine "3000x0" olarak kaydetmek).


# 21 . soru

Görsellik ve veri yönetimi tamam. Oyuna ayak seslerini (Footsteps), blok kırma seslerini ve zombi hırıltılarını eklemek için C++'ta ses sistemini (OpenAL veya SDL_Mixer) nasıl kurarım?


# 21. cevap

Oyununa ses eklemek, hissi tamamen değiştirecek olan o "sihirli" dokunuştur. Sessiz bir dünyada yürümek ile çimenlerin hışırtısını, kırılan taşın yankısını ve arkandan yaklaşan bir zombinin hırıltısını duymak arasında dağlar kadar fark vardır.

Sorunda çok doğru iki teknolojiyi işaret ettin: **SDL_mixer** ve **OpenAL**.

* **SDL_mixer:** Projene eklemesi çok kolaydır (zaten SDL2 kullanıyoruz) ancak temelde **2D** bir ses motorudur. Zombinin sağdan mı yoksa soldan mı geldiğini (3D Spatial Audio) hesaplamak zordur.
* **OpenAL:** 3D sesler için endüstri standartıdır ancak CMake ile derlemesi, kütüphane bağlaması (DLL cehennemi) oldukça zahmetlidir.

Sana kendi oyun motorunu yazan profesyonellerin kullandığı, mimarimizle %100 uyumlu harika bir "gizli silah" önereceğim: **miniaudio**.

Tıpkı `stb_image.h` ve `FastNoiseLite.h` gibi **tek bir `.h` dosyasından oluşur**. CMake'te hiçbir ayar yapmana gerek kalmaz ve içinde OpenAL'den bile daha kolay kullanılabilen kusursuz bir 3D ses motoru (Spatial Audio) barındırır!

İşte adım adım 3D ses sistemini kurmanın yolu:

## 1. Kurulum ve Ses Yöneticisi (AudioEngine)

İnternetten `miniaudio.h` dosyasını indirip `include/` klasörüne at. Ardından projenin her yerinden erişebileceğin bir `AudioEngine` sınıfı oluştur.

```cpp
// src/AudioEngine.cpp (Veya main.cpp'nin üst kısımları)
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <glm/glm.hpp>
#include <iostream>

class AudioEngine {
public:
    ma_engine engine;

    bool Init() {
        if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
            std::cout << "Ses motoru baslatilamadi!" << std::endl;
            return false;
        }
        return true;
    }

    // 2D Sesler için (Envanter açma, Eşya alma, Müzik)
    void Play2DSound(const char* filepath) {
        ma_engine_play_sound(&engine, filepath, NULL);
    }

    // 3D Sesler için (Blok kırma, Zombi, Patlama)
    void Play3DSound(const char* filepath, glm::vec3 pos) {
        // Sesin dünyadaki 3D koordinatını veriyoruz
        ma_engine_play_sound_at_position(&engine, filepath, pos.x, pos.y, pos.z);
    }

    void Cleanup() {
        ma_engine_uninit(&engine);
    }
};

// Global veya ana döngüde tanımlayacağımız nesnemiz
AudioEngine audio;

```

---

## 2. Dinleyicinin (Kameranın) Konumunu Güncellemek

3D ses sistemlerinin bir mantığı vardır: Sesin bir konumu olduğu gibi, duyan kişinin (Kulakların) de bir konumu ve baktığı bir yön vardır. Eğer zombi `X=10` konumundaysa ve sen `X=5` konumundan oraya bakıyorsan sesi tam karşıdan duyarsın. Sağa dönersen, sesi sol kulağından duymaya başlarsın.

`miniaudio` bu karmaşık matematiği senin için otomatik yapar. Tek yapman gereken, her karede (Render döngüsünde) kameranın konumunu ses motoruna bildirmektir:

```cpp
void UpdateAudioListener(Camera& camera) {
    // 0 numaralı dinleyici (Tek oyunculu oynadığımız için)
    
    // Kulaklarımızın dünyadaki yeri
    ma_engine_listener_set_position(&audio.engine, 0, camera.Position.x, camera.Position.y, camera.Position.z);
    
    // Yüzümüzün dönük olduğu yön (Sağ-sol ayrımı için kritik)
    ma_engine_listener_set_direction(&audio.engine, 0, camera.Front.x, camera.Front.y, camera.Front.z);
    
    // Kafamızın yukarı yönü
    ma_engine_listener_set_world_up(&audio.engine, 0, camera.Up.x, camera.Up.y, camera.Up.z);
}

```

Oyun döngünün (Game Loop) içine `UpdateAudioListener(camera);` kodunu eklemeyi unutma.

---

## 3. Sesleri Tetikleme (Mekaniklere Entegrasyon)

Sistemimiz hazır. Artık yapmamız gereken tek şey oyun kodlarımızın arasına doğru tetikleyicileri yerleştirmek. (Projene `assets/sounds/` klasörü açıp içine `.wav` veya `.mp3` dosyalarını koyduğunu varsayıyoruz).

### Ayak Sesleri (Footsteps)

Ayak sesi, oyuncu hareket ediyorsa ve yere basıyorsa belirli aralıklarla (örneğin 0.4 saniyede bir) çalınmalıdır. Bunu `Player` sınıfının fizik güncelleme koduna ekleyebilirsin:

```cpp
float stepTimer = 0.0f;

void UpdateFizik(float deltaTime, World& world) {
    // ... mevcut hareket kodları ...

    // Hızı hesapla (Sadece X ve Z eksenindeki yatay hız)
    float currentSpeed = glm::length(glm::vec2(Velocity.x, Velocity.z));

    // Eğer oyuncu yere basıyorsa ve yürüyorsa
    if (isGrounded && currentSpeed > 0.5f) {
        stepTimer -= deltaTime;
        
        if (stepTimer <= 0.0f) {
            // Hangi bloğun üzerinde yürüdüğümüzü bul
            int blockBelow = world.GetBlock(std::floor(Position.x), std::floor(Position.y - 1.0f), std::floor(Position.z));
            
            if (blockBelow == GRASS_BLOCK || blockBelow == DIRT) {
                audio.Play2DSound("assets/sounds/step_grass.wav");
            } else if (blockBelow == STONE || blockBelow == COBBLESTONE) {
                audio.Play2DSound("assets/sounds/step_stone.wav");
            }
            
            // Oyuncu hızlı koşuyorsa (Sprint) timer'ı daha kısa tutabilirsin
            stepTimer = 0.4f; 
        }
    } else {
        stepTimer = 0.0f; // Durduğunda sayacı sıfırla ki yürür yürümez ses çıksın
    }
}

```

### Blok Kırma Sesi (3D)

`PerformLeftClick` fonksiyonunda (ışının bloğa çarptığı yerde) bloğu sildiğimiz satırın hemen altına:

```cpp
// world.SetBlock(blockX, blockY, blockZ, AIR); // Mevcut kırma kodun

glm::vec3 blockPos(blockX, blockY, blockZ);
audio.Play3DSound("assets/sounds/break_stone.wav", blockPos);

```

Böylece solundaki bir taşı kırdığında sesi sol kulağından, sağındakini kırdığında sağ kulağından duyarsın.

### Zombi Hırıltıları (3D)

Zombiler rastgele aralıklarla bağırmalıdır. `Zombie` sınıfının `UpdateAI` fonksiyonuna bir timer (zamanlayıcı) daha ekleyebilirsin:

```cpp
float groanTimer = (rand() % 5) + 3.0f; // 3 ile 8 saniye arasında rastgele başlangıç

void UpdateAI(float deltaTime, Player& player) override {
    // ... takip kodları ...

    groanTimer -= deltaTime;
    if (groanTimer <= 0.0f) {
        audio.Play3DSound("assets/sounds/zombie_say.wav", position);
        groanTimer = (rand() % 5) + 5.0f; // Bir sonraki hırıltı için süreyi sıfırla
    }
    
    // Zombi hasar aldığında:
    // audio.Play3DSound("assets/sounds/zombie_hit.wav", position);
}

```

Bu sistemi ekleyip kulaklıklarını taktığında, motorun resmen canlanacaktır. Ormanda yürürken duyduğun çimen sesleri ve gece arkanı döndüğünde sol çaprazından gelen zombi sesiyle mükemmel bir hayatta kalma atmosferi elde etmiş olacaksın.


# 22. Soru

Seslerle birlikte atmosfer tamamlandı. Oyuna zaman kavramını ekleyip, gökyüzünün yavaşça karardığı (güneşin batıp yıldızların çıktığı) ve etrafın karanlığa gömüldüğü Gece-Gündüz döngüsünü nasıl yapabilirim?

# 22. cevap

Gece-gündüz döngüsü, oyuncuya zamanın geçtiğini hissettiren ve "gece hayatta kalma" gerilimini başlatan en temel unsurdur.

Bu sistemi motorumuza eklemek için tüm blokların kodunu veya Chunk'ları değiştirmemize gerek yoktur. İşlem tamamen **Matematik** ve **Shader (Gölgelendirici)** katmanında çözülür. Gökyüzünün rengini değiştirecek, ekran kartına "Güneş battı, her şeyi karart" diyecek ve gökyüzüne dönen bir Güneş/Ay çizeceğiz.

İşte zamanı kontrol etmenin adımları:

## 1. Zaman Değişkeni ve Matematik (Game Loop)

Minecraft'ta bir tam gün 24.000 birim (Tik) olarak hesaplanır ve gerçek hayatta tam 20 dakika sürer. Biz de ana oyun döngümüze bu sayacı ekleyip, matematiğin muazzam gücü olan **Sinüs (Sine) dalgasını** kullanarak güneşin yüksekliğini hesaplayacağız.

`main.cpp` dosyanın en üst kısımlarına şu değişkenleri ekle:

```cpp
float timeOfDay = 6000.0f; // Oyun 6000'de (Sabah) başlasın
float timeSpeed = 50.0f;   // Zamanın akış hızı (Bunu artırarak günleri hızlandırabilirsin)

```

Oyun döngünün (Update) içine zamanı ilerleten matematiği kur:

```cpp
// Zamanı ilerlet
timeOfDay += timeSpeed * deltaTime;
if (timeOfDay >= 24000.0f) {
    timeOfDay -= 24000.0f; // Yeni güne başla
}

// Zamanı 0 ile 360 derece (2 * PI Radyan) arasında bir açıya çevir
// 0 = Sabah (0 Derece) | 6000 = Öğlen (90 Derece) | 12000 = Akşam (180 Derece) | 18000 = Gece Yarısı (270 Derece)
float timeAngle = (timeOfDay / 24000.0f) * (2.0f * glm::pi<float>());

// Sinüs kullanarak ışığın şiddetini hesapla
// Öğlen sin(90) = 1.0 (Tam parlaklık), Gece Yarısı sin(270) = -1.0 (Karanlık)
float sunHeight = sin(timeAngle);

// Gece zifiri karanlık olmasın diye ışığı minimum 0.1f (veya %10) ile sınırla
float globalLightIntensity = glm::clamp(sunHeight, 0.1f, 1.0f);

```

---

## 2. Gökyüzü Rengini Değiştirmek (Sky Blending)

Ekranı her karede temizlediğimiz `glClearColor` fonksiyonunu sabitlemek yerine, `globalLightIntensity` değerine göre iki renk arasında yumuşak bir geçiş (Interpolation / Mix) yapmalıyız.

```cpp
// Sabit renkleri belirle
glm::vec3 daySkyColor(0.5f, 0.8f, 1.0f);     // Açık Mavi
glm::vec3 nightSkyColor(0.02f, 0.02f, 0.06f);  // Çok Koyu Lacivert

// Günün saatine göre iki rengi birbirine karıştır
glm::vec3 currentSkyColor = glm::mix(nightSkyColor, daySkyColor, globalLightIntensity);

// Ekranı o anki saate uygun renkle temizle
glClearColor(currentSkyColor.r, currentSkyColor.g, currentSkyColor.b, 1.0f);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

```

Bu sayede güneş batarken gökyüzü yavaş yavaş kararacak, sabah olurken de mükemmel bir pürüzsüzlükte aydınlanacaktır.

---

## 3. Dünya Bloklarını Karartmak (Fragment Shader)

Gökyüzü kararsa bile, ekran kartı blokları hala gündüzmüş gibi parlak çizecektir. Ekran kartına anlık `globalLightIntensity` değerini gönderip, nihai rengi bununla çarpmalıyız.

**3D Çizim Öncesi (C++):**

```cpp
shader3D.use();
shader3D.setFloat("globalLight", globalLightIntensity);

```

**`fragment.glsl` (Güncelleme):**

```glsl
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float Shade; // Orijinal gölgemiz (Ambient Occlusion vb.)

uniform sampler2D ourTexture;
uniform float globalLight; // C++'tan gelen zaman çarpanı

void main() {
    vec4 texColor = texture(ourTexture, TexCoord);
    
    // Rengi Önce kendi gölgesiyle (Shade), sonra Gün Işığıyla (globalLight) çarp!
    vec3 finalColor = texColor.rgb * Shade * globalLight;
    
    FragColor = vec4(finalColor, texColor.a);
}

```

---

## 4. Güneş ve Ayı Gökyüzüne Çizmek (Skybox Mantığı)

Güneşin oyuncu yürüdükçe uzaklaşmaması (her zaman gökyüzünde sabit kalması) için çok özel bir Matris hilesi kullanırız: **Kameranın Görüş Matrisindeki (View Matrix) pozisyon verisini silmek!** Böylece kamera ne kadar ileri giderse gitsin, Güneş hep sonsuz uzaklıkta kalır, sadece farenle sağa sola baktığında döner.

Daha önce Arayüz (UI) için kurduğumuz 1x1'lik Dörtgeni (Quad) bu iş için kullanabiliriz.

```cpp
// 1. Derinlik testini (Z-Buffer) Güneşin dağların arkasında kalması için ayarla
glDepthFunc(GL_LEQUAL); 

// Özel Güneş/Ay Shader'ını aktif et
skyShader.use();
skyShader.setMat4("projection", projectionMatrix);

// 2. SİHİRLİ DOKUNUŞ: Kameranın konumunu sil (Sadece rotasyon kalsın)
// Matrisi mat3'e çevirip tekrar mat4'e alarak X, Y, Z kaydırmalarını sıfırlıyoruz.
glm::mat4 skyView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
skyShader.setMat4("view", skyView);

// 3. GÜNEŞİ DÖNDÜR VE ÇİZ
glm::mat4 sunModel = glm::mat4(1.0f);
// Zaman açısına (timeAngle) göre dünyayı Z ekseni etrafında döndür
sunModel = glm::rotate(sunModel, timeAngle, glm::vec3(0.0f, 0.0f, 1.0f));
// Güneşi yukarıya, gökyüzünün tavanına doğru it (Örn: 100 birim uzaklığa)
sunModel = glm::translate(sunModel, glm::vec3(0.0f, 100.0f, 0.0f)); 
// Güneşi büyüt
sunModel = glm::scale(sunModel, glm::vec3(15.0f, 15.0f, 15.0f));

skyShader.setMat4("model", sunModel);

// Güneş dokusunu bağla ve çiz (Quad)
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, sunTexture);
glBindVertexArray(quadVAO);
glDrawArrays(GL_TRIANGLES, 0, 6);

// 4. AYI ÇİZ (Güneşin tam tersi açısındadır)
glm::mat4 moonModel = glm::mat4(1.0f);
// timeAngle + PI ekleyerek Güneşin 180 derece zıttına yerleştiriyoruz
moonModel = glm::rotate(moonModel, timeAngle + glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
moonModel = glm::translate(moonModel, glm::vec3(0.0f, 100.0f, 0.0f));
moonModel = glm::scale(moonModel, glm::vec3(12.0f, 12.0f, 12.0f)); // Ay biraz daha küçük olabilir

skyShader.setMat4("model", moonModel);
glBindTexture(GL_TEXTURE_2D, moonTexture);
glDrawArrays(GL_TRIANGLES, 0, 6);

// Normale dön
glDepthFunc(GL_LESS);

```

Artık zamanı x50 hızında akıttığında, Güneşin ufuk çizgisinden (dağların arkasından) batışını, ekranın adım adım karanlığa gömülüşünü ve ardından Ayın yükselişini izleyebilirsin!

# 23 . Soru

Gece olunca her şey zifiri karanlığa bürünüyor. Çevreyi aydınlatmak için, yerleştirdiğim bir bloğun (Örn: Meşale) etrafına ışık yaymasını sağlayan Voxel Işıklandırma (Flood Fill Lighting) sistemini nasıl entegre edebilirim?

# 23. cevap

Voxel motorlarında ışıklandırma sistemi (Voxel Lighting), oyun programlamanın en zekice kurgulanmış algoritmalarından biridir. Işık, boşluklarda tıpkı suyun yayıldığı gibi yayılır. Buna **Flood Fill (Taşarak Doldurma)** veya bilgisayar bilimindeki adıyla **Breadth-First Search (BFS - Sığ Öncelikli Arama)** denir.

Minecraft'ta ışık seviyesi **0 ile 15** arasında değişir. Bir meşale koyduğunda o bloğun ışığı 15 olur. Yanındaki hava bloğuna bu ışık 14 olarak geçer, bir sonrakine 13 olarak geçer ve 0 olana kadar yayılmaya devam eder.

İşte bu sistemi motoruna entegre etmenin adım adım yolu:

## 1. Chunk Verisine Işık Haritası (Light Map) Eklemek

Şu ana kadar Chunk'larımızda sadece blok ID'lerini tutuyorduk. Artık her bloğun bir de ışık seviyesine sahip olması gerekiyor.

`Chunk` sınıfına yeni bir dizi ekle:

```cpp
class Chunk {
public:
    uint8_t blocks[16][256][16]; 
    uint8_t lightMap[16][256][16]; // 0 ile 15 arası ışık seviyeleri

    Chunk(int x, int z) {
        // ... mevcut kodlar ...
        
        // Başlangıçta her yeri zifiri karanlık (0) yap
        for(int i = 0; i < 16; i++)
            for(int j = 0; j < 256; j++)
                for(int k = 0; k < 16; k++)
                    lightMap[i][j][k] = 0;
    }
};

```

*(Not: Katı blokların (Taş, Toprak) ışığı her zaman 0'dır, ışık sadece Hava, Yaprak, Su gibi saydam blokların içinde barınabilir.)*

---

## 2. Flood Fill (Taşma) Algoritması (World Sınıfı)

Bir meşale koyduğumuzda ışığın dalga dalga yayılması için bir "Kuyruk" (Queue) kullanmalıyız. Işık, Chunk sınırlarını aşıp yan Chunk'a geçebileceği için bu fonksiyonu doğrudan `World` sınıfının içine yazıyoruz.

Önce kuyrukta tutacağımız basit bir koordinat yapısı oluşturalım:

```cpp
#include <queue>

struct LightNode {
    int x, y, z;
};

```

Şimdi `World` sınıfına BFS (Flood Fill) fonksiyonunu ekleyelim:

```cpp
void PropagateLight(int startX, int startY, int startZ) {
    std::queue<LightNode> lightQueue;
    
    // Işık kaynağını (Meşaleyi) kuyruğa ekle
    lightQueue.push({startX, startY, startZ});

    while (!lightQueue.empty()) {
        LightNode node = lightQueue.front();
        lightQueue.pop();

        // Kuyruktan çektiğimiz bloğun mevcut ışık seviyesini al
        int currentLight = GetLight(node.x, node.y, node.z);

        // Sağ, Sol, Üst, Alt, Ön, Arka (6 Yön) komşuları kontrol et
        int neighbors[6][3] = {
            {node.x + 1, node.y, node.z}, {node.x - 1, node.y, node.z},
            {node.x, node.y + 1, node.z}, {node.x, node.y - 1, node.z},
            {node.x, node.y, node.z + 1}, {node.x, node.y, node.z - 1}
        };

        for (int i = 0; i < 6; i++) {
            int nx = neighbors[i][0];
            int ny = neighbors[i][1];
            int nz = neighbors[i][2];

            // 1. Koordinat dünyadan taşıyor mu? (Örn Y < 0 veya Y > 255)
            if (ny < 0 || ny > 255) continue;

            // 2. Komşu blok katı mı? (Taşın içine ışık sızamaz)
            int blockID = GetBlock(nx, ny, nz);
            // Sadece saydam bloklardan (Hava, Su, Yaprak vb.) ışık geçebilir
            if (blockID != AIR && blockID != OAK_LEAVES /* vb. */) continue;

            // 3. Komşunun mevcut ışığı, bizim yayacağımız ışıktan küçük mü?
            // (Işık her adımda 1 birim azalır)
            if (GetLight(nx, ny, nz) < currentLight - 1) {
                
                // Komşunun ışığını güncelle
                SetLight(nx, ny, nz, currentLight - 1);
                
                // Komşuyu da kuyruğa ekle ki o da kendi komşularını aydınlatsın!
                lightQueue.push({nx, ny, nz});
            }
        }
    }
}

```

**Oyuna Meşale Koyduğunda Çalışacak Kod:**

```cpp
// 1. Bloğu yerleştir
world.SetBlock(x, y, z, TORCH_ID);
// 2. Işığını maksimum (15) yap
world.SetLight(x, y, z, 15);
// 3. Işığı etrafa yay!
world.PropagateLight(x, y, z);

```

> **Önemli Not:** Işık yayılma işlemi bittiğinde, ışığın ulaştığı (değişiklik yapılan) tüm Chunk'ların `GenerateMesh()` ve `BufferData()` fonksiyonlarını yeniden çağırarak ekran kartını güncellemelisin. Aksi takdirde ışığı ekranda göremezsin.

---

## 3. Ekran Kartına Işık Seviyesini Göndermek

Tıpkı ortam gölgelendirmesi (Ambient Occlusion) değerini gönderdiğimiz gibi, artık her yüzeyin **blok ışık seviyesini** de Vertex'lerimize eklemeliyiz.

`Chunk::GenerateMesh` içinde, çizdiğin yüzeyin dışındaki (yanındaki) boşluğun ışık seviyesini okumalı ve bunu listeye eklemelisin.

```cpp
// Örnek: Üst yüzeyi (Y+1) çizerken, Y+1 koordinatındaki bloğun ışık seviyesini al
float blockLightLevel = (float)world.GetLight(globalX, globalY + 1, globalZ) / 15.0f; // 0.0f ile 1.0f arasına çevir

// Listeye eklerken artık U, V, Shade ve Light verilerini yolluyoruz
meshVertices.insert(meshVertices.end(), {
    x-0.5f, y+0.5f, z+0.5f,  uMin, vMin,  shade00, blockLightLevel, // Sol Alt
    // Diğer köşeler...
});

```

*(Not: Attribute Pointer ayarlarını bu yeni parametreye göre güncellemeyi unutma!)*

---

## 4. Gündüz ve Geceyi Shader'da Birleştirmek (Sihir Zamanı)

Ekran kartındaki Gölgelendirici (Shader), oyunun asıl büyüsünün koptuğu yerdir.
Bir bloğun nihai parlaklığı, **Güneş Işığı (Global Light)** ile **Meşale Işığı (Block Light)** arasındaki **EN YÜKSEK (Max)** değere eşittir.

Yani eğer bir mağaradaysan güneş ışığı oraya ulaşmaz ama meşalenin ışığı ortamı aydınlatır. Öğlen vakti meşale koyarsan, güneş ışığı (1.0) meşalenin ışığını ezeceği için meşalenin bir etkisi kalmaz.

**`vertex.glsl` Güncellemesi:**

```glsl
layout (location = 3) in float aBlockLight; // C++'tan gelen Meşale ışığı
out float BlockLight;

void main() {
    // ...
    BlockLight = aBlockLight;
}

```

**`fragment.glsl` Güncellemesi:**

```glsl
in float Shade;       // Çevresel Gölgeler (AO)
in float BlockLight;  // Bu bloğun üzerindeki Meşale ışığı (0.0 - 1.0)

uniform sampler2D ourTexture;
uniform float globalLight; // C++'tan gelen zaman çarpanı (Güneş Işığı)

void main() {
    vec4 texColor = texture(ourTexture, TexCoord);
    
    // Güneş ışığı ile meşale ışığından hangisi daha güçlüyse onu kullan
    float finalLightLevel = max(globalLight, BlockLight);
    
    // Minimum bir parlaklık ekle (Zifiri karanlıkta oyuncu hiçbir şey göremez)
    finalLightLevel = max(finalLightLevel, 0.05);

    vec3 finalColor = texColor.rgb * Shade * finalLightLevel;
    
    FragColor = vec4(finalColor, texColor.a);
}

```

### Meşaleyi Kırarken Ne Olacak? (Light Removal)

Meşaleyi kırdığında ışığı geri çekmek için, ışığı yayan kaynağı sildikten sonra eski ışık seviyesini kaydedersin. Etraftaki blokları kontrol eder, eğer o blokların ışığı meşaleden geliyorsa onları da 0 yapar ve bir "Karanlık Kuyruğuna" (Removal Queue) eklersin. Karanlık yayılıp bittiğinde, sınırda kalan (başka kaynaklardan aydınlanan) ışıkları bulup onlardan tekrar bir `PropagateLight` dalgası başlatırsın.

Bu BFS sistemiyle karanlık bir mağaraya girip elindeki meşaleyi duvara yerleştirdiğinde, ışığın pürüzsüz bir dalga halinde mağara duvarlarına vurduğunu ve gecenin karanlığını deldiğini göreceksin!


# 24. soru

Işıklandırmamız tamamlandı. Ormanlarda ve vadilerde nehirler oluşturmak (Su fiziği) ve yapraklar/camlar arkasından dünyayı doğru şekilde görebilmek (Alpha Blending/Derinlik sıralaması) için ne yapmalıyım?


# 24. cevap

Harika bir ilerleme! Su, yaprak ve cam gibi bloklar eklendiğinde oyun motoru tamamen nefes almaya başlar. Ancak bu adımda, 3D grafik programlamanın en meşhur baş ağrısı olan **Z-Buffer (Derinlik Belleği) Çakışması** ile tanışma vaktimiz geldi.

Eğer şeffaf bir cam bloğunu, arkasındaki toprak bloğundan *önce* çizersen; ekran kartı o piksele camın derinliğini (Z ekseni) kaydeder. Sıra arkadaki toprağı çizmeye geldiğinde, ekran kartı *"Bu toprak camdan daha uzakta, o zaman onu çizmeme gerek yok"* diyerek toprağı siler ve camın arkasında devasa bir gökyüzü boşluğu görürsün.

Özellikle oyun motorunu Android gibi donanım gücü sınırlı platformlara taşımayı hedeflediğimiz için, ekran kartını yoran karmaşık derinlik sıralamalarından (Depth Sorting) kaçınmalıyız. Çözüm, blokları üç farklı davranış kategorisine ayırmaktır:

## 1. Blok Kategorileri

* **Opaque (Katı):** Toprak, Taş, Odun. Işık geçirmezler. (İlk çizilirler)
* **Cutout (Delikli):** Yaprak, Cam. Pikselleri ya tamamen saydamdır ya da tamamen katıdır (Yarı saydamlık yoktur). (Katılarla aynı anda çizilirler, Shader hilesi kullanılır).
* **Translucent (Yarı Saydam):** Su, Buz. Arkasını renkli bir filtreyle gösterir. Şeffaflık (Alpha) hesaplaması gerektirir. (En son çizilirler).

---

## 2. Yapraklar ve Cam İçin Çözüm: "Discard" Hilesi

Yaprak veya cam bloklarının saydam kısımlarının arkasını göstermesi için RAM'i yoran sıralamalara girmemize hiç gerek yoktur. Fragment Shader (Renk Gölgelendiricisi) içinde saydam pikselleri "çöpe atarak" (discard) derinlik testini kandırabiliriz.

`fragment.glsl` dosyanı şu şekilde güncelle:

```glsl
#version 330 core
out vec4 FragColor;
// ... diğer değişkenler ...

void main() {
    vec4 texColor = texture(ourTexture, TexCoord);
    
    // CUTOUT HİLESİ: Eğer pikselin Alpha (Saydamlık) değeri çok düşükse, bu pikseli HİÇ ÇİZME!
    // Böylece Z-Buffer'a kaydedilmez ve arkasındaki bloklar görünür kalır.
    if(texColor.a < 0.1) {
        discard; 
    }
    
    vec3 finalColor = texColor.rgb * Shade * globalLight;
    FragColor = vec4(finalColor, texColor.a);
}

```

Bu tek satırlık kod sayesinde yaprakların aralarındaki boşluklardan gökyüzünü veya ağacın arkasındaki dağları kusursuzca görebilirsin.

---

## 3. Su ve Yarı Saydamlar: İkili Çizim Sistemi (Two-Pass Rendering)

Suyun arkasını mavi bir filtreyle görmek (örneğin Alpha değeri `0.6`) `discard` ile yapılamaz. Çözüm: Chunk içindeki katı bloklarla su bloklarını **iki ayrı listede** tutup, ekran kartına iki ayrı VAO (Çizim Paketi) olarak göndermektir.

`Chunk` sınıfını şu şekilde güncelle:

```cpp
class Chunk {
public:
    // Mevcut listemiz (Taş, Toprak, Yaprak, Cam için)
    std::vector<float> opaqueMeshVertices;
    
    // Su ve yarı saydamlar için YENİ listemiz
    std::vector<float> transparentMeshVertices; 
    
    unsigned int opaqueVAO, opaqueVBO;
    unsigned int transparentVAO, transparentVBO;

    // ...
};

```

**Mesh Üretim (Face Culling) Kuralları:**
Suları veya camları birbirlerine bağlarken yüzeylerin silinmesi, ancak farklı bir bloğa değdiklerinde silinmemesi için culling mantığını esnetmeliyiz.

```cpp
// Bir yüzeyin çizilip çizilmeyeceğine karar veren yardımcı fonksiyon
bool ShouldRenderFace(int currentBlockID, int neighborBlockID) {
    if (neighborBlockID == AIR) return true; // Yanı hava ise kesin çiz
    
    if (currentBlockID == WATER) {
        // Su, başka bir suyun yanındaysa aradaki yüzeyi ÇİZME (Göller pürüzsüz olsun)
        if (neighborBlockID == WATER) return false;
        // Su, katı bir bloğun veya camın yanındaysa yüzeyini ÇİZ
        return true;
    }
    
    if (currentBlockID == GLASS || currentBlockID == OAK_LEAVES) {
        // İki cam veya iki yaprak yan yanaysa aradaki yüzeyi sil
        if (neighborBlockID == currentBlockID) return false;
    }

    // Komşu blok saydam bir şeyse (Su, Cam, Yaprak), mevcut katı bloğun yüzeyini ÇİZ!
    if (neighborBlockID == WATER || neighborBlockID == GLASS || neighborBlockID == OAK_LEAVES) {
        return true; 
    }

    return false; // İki katı blok yan yanaysa çizme
}

```

Mesh'i üretirken, eğer `currentBlockID == WATER` ise hesapladığın yüzey koordinatlarını `transparentMeshVertices` listesine eklersin. Değilse `opaqueMeshVertices` listesine eklersin.

**Render Döngüsü (Kusursuz Şeffaflık İçin):**

```cpp
// 1. GEÇİŞ: Sadece Katı ve Cutout Blokları Çiz (Derinlik testi AÇIK)
for(auto& chunk : activeChunks) {
    glBindVertexArray(chunk->opaqueVAO);
    glDrawArrays(GL_TRIANGLES, 0, chunk->opaqueMeshVertices.size() / 7);
}

// 2. GEÇİŞ: Yarı Saydamları Çiz
// Derinlik testini tamamen kapatmıyoruz, sadece "Yazmayı" (Z-Write) kapatıyoruz
// Böylece su, toprağın arkasındaysa çizilmez, ama önündeyse çizilir ve kendi aralarında çakışmazlar.
glDepthMask(GL_FALSE); 

for(auto& chunk : activeChunks) {
    if (chunk->transparentMeshVertices.size() > 0) {
        glBindVertexArray(chunk->transparentVAO);
        glDrawArrays(GL_TRIANGLES, 0, chunk->transparentMeshVertices.size() / 7);
    }
}

// Bir sonraki kare için Z-Write'ı tekrar aç
glDepthMask(GL_TRUE); 

```

---

## 4. Vadilerde Nehirler ve Denizler Oluşturmak

Perlin Noise ile araziyi oluşturduğumuz kısıma "Deniz Seviyesi" (Sea Level) adında küresel bir değişken ekliyoruz. Eğer üretilen arazi bu seviyenin altındaysa, aradaki boşlukları hava yerine suyla dolduruyoruz.

`Chunk::GenerateTerrain` fonksiyonunda y eksenini doldurduğumuz döngüyü güncelle:

```cpp
int seaLevel = 60; // 60. yüksekliğe kadar olan tüm çukurlar suyla dolsun

for(int y = 0; y < 256; y++) {
    if (y == 0) {
        blocks[x][y][z] = BEDROCK;
    } 
    else if (y <= terrainHeight) {
        // Arazi blokları (Toprak, Taş vb. Biyoma göre karar ver)
        blocks[x][y][z] = ...; 
    } 
    else if (y > terrainHeight && y <= seaLevel) {
        // EĞER arazi deniz seviyesinin altında kaldıysa ve burası havaysa:
        blocks[x][y][z] = WATER;
    } 
    else {
        blocks[x][y][z] = AIR;
    }
}

```

> **Görsel Bir Hile (Su Seviyesi):** Suların tam bir blok (1.0f) yüksekliğinde olması gerçekçi durmaz. Mesh üretiminde suyu çizerken, üst yüzeyinin (Y+1) Y koordinatından `0.2f` çıkararak suların normal bloklardan biraz daha alçakta durmasını sağlarsan, göl ve nehir kıyıları görsel olarak muazzam bir hacim kazanır.



# 25. soru

Yeryüzü şekillenip nehirler aktığına göre, yerin altına inip devasa mağara sistemlerini (Perlin Worms/3D Noise) ve elmas, demir gibi maden damarlarını nasıl oluşturabilirim?


# 25. cevap

Yeryüzündeki dağları ve nehirleri oluştururken **2 Boyutlu (2D) Noise** kullandık; çünkü sadece X ve Z koordinatlarına bakarak bir "Yükseklik" haritası çıkarmamız yetiyordu.

Ancak yerin altındaki mağaralar ve maden damarları için bir yüksekliğe değil, **Hacme (Volume)** ihtiyacımız var. Bloğun X, Y ve Z koordinatlarının üçünü birden hesaplayan **3 Boyutlu (3D) Noise** kullanarak yerin altını adeta bir İsviçre peyniri gibi oyacağız.

İşte devasa mağaraları ve derinlere indikçe nadirleşen madenleri oluşturmanın formülü:

## 1. Yeni Blok ID'lerini Eklemek

Öncelikle daha önce oluşturduğumuz `Registry.h` dosyasına madenlerimizi ekleyelim:

```cpp
enum ItemID {
    // ... önceki bloklar ...
    COAL_ORE = 18, IRON_ORE = 19, GOLD_ORE = 20, DIAMOND_ORE = 21,
    // ...
};

```

---

## 2. 3D Noise Kurulumu (FastNoiseLite)

Mağaralar ve madenler için `Chunk::GenerateTerrain` fonksiyonumuzun içine yeni `FastNoiseLite` nesneleri eklemeliyiz. Mağaralar için "Spagetti" (Solucan) benzeri tüneller oluşturan özel bir ayar kullanacağız.

```cpp
void GenerateTerrain() {
    // Mevcut Yeryüzü Noise'ları
    FastNoiseLite heightNoise;
    heightNoise.SetFrequency(0.01f);

    // 1. MAĞARALAR İÇİN 3D NOISE
    FastNoiseLite caveNoise;
    caveNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2); 
    caveNoise.SetFrequency(0.03f); // Frekans büyüdükçe mağaralar daralır ve kıvrımlı olur
    
    // Mağaraların tünel (solucan) gibi uzaması için Ridged (Sırtlı) fractal tipini kullanıyoruz
    caveNoise.SetFractalType(FastNoiseLite::FractalType_Ridged); 

    // 2. MADEN DAMARLARI İÇİN 3D NOISE
    FastNoiseLite oreNoise;
    oreNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular); // Hücresel yapı damarlar için mükemmeldir
    oreNoise.SetFrequency(0.1f); // Küçük ve sık damarlar

```

---

## 3. Üretim Döngüsünü (Yeraltını) Güncellemek

Daha önce oluşturduğumuz X, Z ve Y döngülerinin içinde, arazi (terrainHeight) hesaplandıktan sonra yerin altını doldurduğumuz kısmı baştan aşağı değiştiriyoruz.

Oyun motorlarında **üretim hiyerarşisi** çok önemlidir: Önce taşı koyarız, sonra taşı madene dönüştürürüz, en son mağara gürültüsü "Burada boşluk olmalı" derse her ikisini de silip havaya (AIR) dönüştürürüz.

```cpp
    for(int x = 0; x < 16; x++) {
        for(int z = 0; z < 16; z++) {
            
            float globalX = (chunkX * 16.0f) + x;
            float globalZ = (chunkZ * 16.0f) + z;
            int terrainHeight = 64 + (int)(heightNoise.GetNoise(globalX, globalZ) * 30.0f);

            for(int y = 0; y < 256; y++) {
                
                // 1. BEDROCK (Kırılmaz Kaya) - En Alt Katman
                if (y == 0) {
                    blocks[x][y][z] = BEDROCK;
                    continue;
                }
                
                // 2. GÖKYÜZÜ VE YERYÜZÜ (Önceki kodlarımız)
                if (y > terrainHeight) {
                    if (y <= 60) blocks[x][y][z] = WATER; // Deniz seviyesi
                    else blocks[x][y][z] = AIR;
                    continue;
                }
                
                if (y == terrainHeight) {
                    blocks[x][y][z] = GRASS_BLOCK; // Yüzey
                    continue;
                }
                if (y < terrainHeight && y > terrainHeight - 4) {
                    blocks[x][y][z] = DIRT; // Yüzeyin hemen altı
                    continue;
                }

                // 3. YERALTI: TAŞ, MADENLER VE MAĞARALAR
                if (y <= terrainHeight - 4) {
                    
                    // A. MAĞARA HESAPLAMASI (3D NOISE)
                    // Yeraltındaysak 3 boyutlu gürültüyü hesapla
                    float caveValue = caveNoise.GetNoise(globalX, (float)y, globalZ);
                    
                    // Ridged Noise 0.0 ile 1.0 arası döner. 
                    // Eğer değer 0.05'ten küçükse (İnce bir tünel hattı), burayı BOŞLUK (Mağara) yap.
                    if (caveValue < 0.05f) {
                        blocks[x][y][z] = AIR;
                        continue; // Mağara olduysa maden koymaya gerek yok, döngüyü atla
                    }

                    // B. MADEN DAMARLARI (MAĞARA DEĞİLSE)
                    float oreValue = oreNoise.GetNoise(globalX, (float)y, globalZ);
                    
                    // Madenlerin Yüksekliğe Göre Dağılımı (Derinlere indikçe değerler değişir)
                    if (oreValue > 0.85f) {
                        // Kömür her yerde bulunabilir
                        blocks[x][y][z] = COAL_ORE; 
                    } 
                    else if (oreValue < -0.85f && y < 64) {
                        // Demir 64. yüksekliğin altında
                        blocks[x][y][z] = IRON_ORE;
                    } 
                    else if (oreValue > 0.75f && oreValue < 0.80f && y < 32) {
                        // Altın 32'nin altında
                        blocks[x][y][z] = GOLD_ORE;
                    } 
                    else if (oreValue > 0.70f && oreValue < 0.72f && y < 16) {
                        // Elmas sadece en derinlerde (Y < 16) ve çok nadir (Dar bir eşik)
                        blocks[x][y][z] = DIAMOND_ORE;
                    } 
                    else {
                        // Hiçbirine uymuyorsa standart taş koy
                        blocks[x][y][z] = STONE;
                    }
                }
            }
        }
    }
}

```

---

## Performans İpucu (CPU Optimizasyonu)

Dikkat edersen `GetNoise(X, Y, Z)` fonksiyonunu gökyüzündeki hava blokları için hiç çağırmadık. Sadece `y <= terrainHeight - 4` koşulunun içinde çalıştırdık.

3 Boyutlu matematik, 2 boyutlu matematiğe göre işlemciyi çok daha fazla yorar. Eğer bu fonksiyonu her Y döngüsünde (Havada bile) çağırsaydın, Chunk üretim süren (Mesh Generation) 3 katına çıkar ve oyun sürekli anlık takılmalar (Lag Spike) yaşardı.

Motoru çalıştırıp bir mağara ağzından aşağı atladığında (veya taşı kazarak aşağı indiğinde), tünellerin kıvrılarak birbirine bağlandığını, derinlere indikçe taşların arasında parlayan elmasları ve demir damarlarını doğal yığınlar halinde (chunk sınırlarında bile kesilmeden) göreceksin!


# 26. soru

Oyunda eşyalarımızı ve madenleri toplayabildiğimize göre, bir Çalışma Masası (Crafting Table) arayüzü açarak topladığım odunlardan kılıç veya kazma yapmayı sağlayan Üretim sistemini nasıl kurabilirim?


# 26. cevap

Üretim (Crafting) sistemi, oyuncunun dünyadan topladığı ham maddeleri işe yarar araçlara dönüştürdüğü, oyunun temel döngüsünü (Game Loop) tamamlayan en can alıcı mekaniktir.

Crafting sistemi yaparken, 3D dünyadan tamamen çıkıp bir **Veri Tabanı Eşleşmesi (Pattern Matching)** ve **Arayüz Yönetimi (UI Management)** mantığına geçeriz. Oyuncu 3x3'lük bir ızgaraya eşyaları dizer ve motorumuz arka planda *"Bu dizilim benim tarif defterimdeki herhangi bir şeye uyuyor mu?"* diye kontrol eder.

İşte profesyonel bir Crafting sistemini motoruna kurmanın adımları:

## 1. Yeni Eşyaları ve Tarif Yapısını Tanımlamak

Önce `Registry.h` dosyamıza yapacağımız eşyaları (Çubuk, Kazma vb.) eklemeliyiz. Sonra da oyunun başlangıcında RAM'e yüklenecek olan `Recipe` (Tarif) yapısını oluşturacağız.

```cpp
// Registry.h içine eklenecekler
enum ItemID {
    // ... öncekiler ...
    CRAFTING_TABLE = 50,
    STICK = 260, WOODEN_PICKAXE = 261, WOODEN_SWORD = 262
};

// Tarifleri tutacağımız yapı
struct Recipe {
    int ingredients[9]; // 3x3'lük üretim ızgarası (Soldan sağa, yukarıdan aşağıya)
    int resultID;       // Çıkacak eşya
    int resultCount;    // Çıkacak eşyanın adedi
};

// Tüm tariflerin tutulduğu global listemiz
std::vector<Recipe> recipeDatabase;

```

Oyun başlarken (`Init` aşamasında) tarifleri bu listeye eklemelisin. Örneğin, Ahşap Kazma (Wooden Pickaxe) tarifi:

```cpp
void LoadRecipes() {
    // 0 = Boş (Hava). 
    // Ahşap Kazma Tarifi (Üstte 3 tahta, ortada 2 çubuk)
    Recipe pickaxe;
    pickaxe.ingredients[0] = OAK_PLANKS; pickaxe.ingredients[1] = OAK_PLANKS; pickaxe.ingredients[2] = OAK_PLANKS;
    pickaxe.ingredients[3] = AIR;        pickaxe.ingredients[4] = STICK;      pickaxe.ingredients[5] = AIR;
    pickaxe.ingredients[6] = AIR;        pickaxe.ingredients[7] = STICK;      pickaxe.ingredients[8] = AIR;
    pickaxe.resultID = WOODEN_PICKAXE;
    pickaxe.resultCount = 1;
    recipeDatabase.push_back(pickaxe);
    
    // Çubuk Tarifi (Üst üste 2 tahta)
    // Not: Normalde 3x3 gridin herhangi bir yerinde olabilir ama basitleştirmek için 
    // sol alt köşeye dayalı (veya ortaya) kesin koordinatlar verebiliriz.
}

```

---

## 2. Üretim Izgarası (Crafting Grid) Değişkenleri

Oyuncu Çalışma Masasına (Crafting Table) sağ tıkladığında oyun duraklamaz, sadece ekrana yeni bir arayüz çizilir. Bu arayüzün arka planında verileri tutacak yeni dizilere ihtiyacımız var.

`Player` veya arayüzü yöneten sınıfının içine şunları ekle:

```cpp
// 3x3'lük üretim masası yuvaları
InventorySlot craftingGrid[9]; 

// Üretim sonucunun (Kazmanın) belireceği sonuç yuvası
InventorySlot craftingResult; 

// Arayüz açık mı?
bool isCraftingMenuOpen = false;

// Sürükle-bırak için oyuncunun faresine "yapışık" olan eşya
InventorySlot mouseSlot; 

```

---

## 3. Tarif Eşleştirme Algoritması (Core Logic)

Crafting sisteminin kalbi burasıdır. Oyuncu üretim ızgarasındaki herhangi bir yuvaya bir eşya koyduğunda veya aldığında **anında** bu fonksiyonu çağırmalısın. Fonksiyon, ızgaradaki dizilimin veri tabanımızdaki bir tarifle eşleşip eşleşmediğine bakar.

```cpp
void CheckCraftingRecipes() {
    // Önce sonuç yuvasını boşalt (Eşleşme bozulmuş olabilir)
    craftingResult.blockID = AIR;
    craftingResult.count = 0;

    // Veri tabanındaki tüm tarifleri dön
    for (const Recipe& recipe : recipeDatabase) {
        bool match = true;

        // 3x3 (9 yuva) kontrolü
        for (int i = 0; i < 9; i++) {
            // Eğer tarifteki ID ile ızgaradaki ID uyuşmuyorsa bu tarif iptal!
            if (craftingGrid[i].blockID != recipe.ingredients[i]) {
                match = false;
                break;
            }
        }

        // Eğer 9 yuvanın hepsi uyuştuysa (Match bozulmadıysa)
        if (match) {
            // Sonuç yuvasına eşyayı koy ve döngüden çık
            craftingResult.blockID = recipe.resultID;
            craftingResult.count = recipe.resultCount;
            return;
        }
    }
}

```

---

## 4. Eşya Üretme İşlemi (Sonuç Yuvasına Tıklamak)

Eğer algoritma eşleşmeyi bulduysa ve sonuç yuvasında bir Ahşap Kazma belirdiyse, oyuncu farenin sol tıkıyla bu kazmayı alır. Kazma alındığı anda ızgaradaki (craftingGrid) tüm ham maddelerin sayısını birer tane azaltmalıyız.

Mouse ile tıklama etkileşimi:

```cpp
// Eğer oyuncu Sonuç Yuvasına (craftingResult) sol tıklarsa:
if (craftingResult.blockID != AIR) {
    
    // 1. Üretilen eşyayı farenin ucuna (veya doğrudan envantere) al
    mouseSlot.blockID = craftingResult.blockID;
    mouseSlot.count = craftingResult.count;
    
    // 2. Izgaradaki (9 yuva) ham maddelerin sayısını azalt
    for (int i = 0; i < 9; i++) {
        if (craftingGrid[i].blockID != AIR) {
            craftingGrid[i].count--;
            
            // Eğer eşya bittiyse (0 olduysa), yuvayı tamamen boşalt
            if (craftingGrid[i].count <= 0) {
                craftingGrid[i].blockID = AIR;
            }
        }
    }
    
    // 3. Izgara değiştiği için eşleşmeyi tekrar kontrol et
    CheckCraftingRecipes();
}

```

---

## 5. Mouse Sürükle-Bırak ve Arayüz Çizimi

Oyun döngünde (Game Loop) eğer `isCraftingMenuOpen` değişkeni `true` ise:

1. Farenin 3D kamerayı döndürmesini sağlayan fonksiyonu devre dışı bırak.
2. Fare imlecini ekranda görünür yap (`SDL_ShowCursor(SDL_ENABLE)`).
3. Fare koordinatlarını (X, Y) okuyarak, ekrana çizdirdiğin 3x3'lük Arayüz kutularından hangisinin üzerine denk geldiğini matematiksel olarak bul (Hitbox mantığının 2 boyutlu versiyonu).
4. Ekrana `DrawUIElement` ile envanteri, 3x3 ızgarayı ve sonuç yuvasını çizdir. En son olarak da, eğer farenin ucunda bir eşya varsa (`mouseSlot.blockID != AIR`), farenin anlık (X,Y) koordinatına o eşyanın ikonunu çizdir ki oyuncu eşyayı elinde tuttuğunu hissetsin.

**Küçük Bir Güvenlik Kuralı:** Oyuncu ESC tuşuna basıp masadan kalktığında, `craftingGrid` içinde kalan eşyaları oyuncunun etrafına (dünyaya) fırlatmayı veya envanterine geri koymayı unutma. Yoksa masayı kapatan oyuncunun eşyaları sonsuza dek silinir!


# 27. soru

Kazmayı ürettiğimize göre, elime kazma aldığımda taşları çok hızlı kırmamı ancak toprakları yavaş kırmamı sağlayan 'Alet/Blok Hızı' ve 'Dayanıklılık' (Durability) sistemini nasıl ekleyebilirim?


# 27. cevap

Aletlerin madenlere göre farklı hızlarda çalışması ve kırılması, oyununun mekaniklerine devasa bir strateji katmanı ekler. Şu ana kadar blokları sol tıkladığımız an saniyesinde kırıyorduk. Bu sistemi kurmak için "Anında Kırma" (Instant Break) mantığından çıkıp, **"Basılı Tutarak Kırma" (Hold to Break)** mantığına geçmeliyiz.

Bunu başarmak için bloklara bir "Sertlik" (Hardness), aletlere ise "Verimlilik" (Efficiency) ve "Dayanıklılık" (Durability) değerleri atayacağız.

## 1. Eşya ve Blok Veri Tabanını (Registry) Genişletmek

Sadece ID'leri tutmak artık yetmez. Oyun başladığında belleğe yüklenecek olan blok ve eşya özelliklerini tutan bir yapı kurmalıyız.

```cpp
// Hangi aletin hangi bloğa uygun olduğunu belirlemek için
enum ToolType { TOOL_NONE, TOOL_PICKAXE, TOOL_AXE, TOOL_SHOVEL, TOOL_SWORD };

struct BlockData {
    float hardness;      // Bloğun kırılma zorluğu (Örn: Toprak 0.5f, Taş 1.5f, Obsidian 50.0f)
    ToolType bestTool;   // Bu bloğu en hızlı hangi alet kırar?
};

struct ItemData {
    ToolType toolType;       // Bu eşya ne tür bir alet?
    float speedMultiplier;   // Kırma hızı çarpanı (Tahta: 2.0f, Elmas: 8.0f)
    int maxDurability;       // Maksimum dayanıklılık (Tahta: 59, Demir: 250)
};

// Global veri tabanlarımız (Oyun başlarken doldurulur)
std::unordered_map<int, BlockData> blockRegistry;
std::unordered_map<int, ItemData> itemRegistry;

```

Oyun başlatılırken (Init aşamasında) bunları şöyle tanımlarsın:

```cpp
// Taş bloğunun özellikleri
blockRegistry[STONE] = { 1.5f, TOOL_PICKAXE };
blockRegistry[DIRT] = { 0.5f, TOOL_SHOVEL };

// Ahşap Kazma özellikleri
itemRegistry[WOODEN_PICKAXE] = { TOOL_PICKAXE, 2.0f, 59 };
itemRegistry[STONE_SWORD] = { TOOL_SWORD, 1.0f, 131 }; // Kılıç blok kırmada hızlı değildir

```

---

## 2. Envanter Yuvasına "Dayanıklılık" Eklemek

`InventorySlot` yapımızı, eşyanın ne kadar hasar aldığını (veya ne kadar ömrü kaldığını) hatırlayacak şekilde güncellemeliyiz.

```cpp
struct InventorySlot {
    int blockID;
    int count;
    int currentDurability; // YENİ: Aletin anlık canı
};

```

*Not: Üretim (Crafting) sisteminde yeni bir alet yaptığında, `currentDurability` değerini `itemRegistry[aletID].maxDurability` ile başlatmayı unutma.*

---

## 3. Basılı Tutarak Kırma Matematiği (Game Loop)

Artık `PerformLeftClick` fonksiyonumuz farenin sol tuşuna *tıklandığı an* değil, *basılı tutulduğu sürece* (her karede) çalışmalıdır. Aynı bloğa ne kadar süredir baktığımızı takip eden bir sayaca (Timer) ihtiyacımız var.

`Player` veya `World` sınıfına şu değişkenleri ekle:

```cpp
glm::vec3 breakingBlockPos(-1, -1, -1); // Şu an hangi bloğu kırıyoruz?
float breakingProgress = 0.0f;          // Kaç saniyedir kırıyoruz?

```

Oyun döngüsündeki blok kırma mantığını şu şekilde değiştir:

```cpp
void UpdateMining(float deltaTime, Player& player, World& world) {
    // Farenin sol tuşuna BASILI TUTULMUYORSA kırma işlemini sıfırla
    if (!mouse.LeftButtonHeld) {
        breakingProgress = 0.0f;
        breakingBlockPos = glm::vec3(-1, -1, -1);
        return;
    }

    // 1. Raycasting ile baktığımız bloğu bul (Önceki yazdığımız Işın İzleme fonksiyonu)
    glm::vec3 targetBlock = GetTargetBlockFromRaycast(player.Position, camera.Front);
    
    // Gökyüzüne bakıyorsak iptal et
    if (targetBlock.y < 0) {
        breakingProgress = 0.0f;
        return;
    }

    // Eğer fareyi kaydırıp BAŞKA bir bloğa bakmaya başlarsak sayacı sıfırla
    if (targetBlock != breakingBlockPos) {
        breakingProgress = 0.0f;
        breakingBlockPos = targetBlock;
    }

    // 2. Bloğun ve Elimizdeki Aletin Verilerini Çek
    int targetBlockID = world.GetBlock(targetBlock.x, targetBlock.y, targetBlock.z);
    int activeItemID = player.GetActiveItem(); // Hotbar'da seçili eşya

    BlockData bData = blockRegistry[targetBlockID];
    ItemData iData = itemRegistry[activeItemID];

    // 3. Kırma Hızını Hesapla
    float mineSpeed = 1.0f; // Çıplak el hızı

    // Elimizdeki alet, bu blok için "Doğru Alet" ise hızı çarp
    if (bData.bestTool != TOOL_NONE && bData.bestTool == iData.toolType) {
        mineSpeed = iData.speedMultiplier;
    } else {
        // Eğer yanlış aletse (Örn: Kazmayla odun kırmak) veya çıplak else hız 1.0 kalır
        // Hatta Minecraft'ta kazmayla odun kırmak çıplak elden daha hızlı değildir.
    }

    // 4. Kırma Sürecini İlerlet
    // Formül: (Kırma Hızı / Bloğun Sertliği) * Saniye
    breakingProgress += (mineSpeed / bData.hardness) * deltaTime;

    // 5. BLOK KIRILDI MI?
    if (breakingProgress >= 1.5f) { // 1.5f standart Minecraft kırma eşiğidir
        
        // Bloğu haritadan sil
        world.SetBlock(targetBlock.x, targetBlock.y, targetBlock.z, AIR);
        
        // Ses çal ve parçacık efekti oluştur...
        
        // 6. ALETİN DAYANIKLILIĞINI DÜŞÜR
        if (iData.toolType != TOOL_NONE) {
            player.inventory[player.activeSlotIndex].currentDurability--;
            
            // Alet kırıldıysa (Dayanıklılık 0 olduysa)
            if (player.inventory[player.activeSlotIndex].currentDurability <= 0) {
                // Aleti envanterden sil (Kırılma sesi çalınabilir)
                player.inventory[player.activeSlotIndex].blockID = AIR;
                player.inventory[player.activeSlotIndex].count = 0;
            }
        }
        
        // Bir sonraki blok için sayacı sıfırla
        breakingProgress = 0.0f;
        breakingBlockPos = glm::vec3(-1, -1, -1);
    }
}

```

---

## 4. Görsel Geri Bildirim: Çatlama Animasyonu (Block Damage)

Dayanıklılık ve hız sistemi çalıştığında oyuncuya görsel bir ipucu vermezsen, bloğun ne zaman kırılacağını anlayamaz ve oyun hissiyatı zayıflar. Minecraft'taki gibi blok kırılırken üzerinde oluşan "çatlak" dokusunu (destroy_stage_0.png - 9.png) eklemek muazzam bir detaydır.

Bu çatlak dokuları genellikle şeffaf arka plana sahip 10 aşamalı bir resimdir.

**Nasıl Çizdirilir?**
Çizim döngüsünde (Render), 3D dünyayı çizdikten hemen sonra ama 2D Arayüzü çizmeden önce, sadece `breakingBlockPos` koordinatının üzerine, o anki `breakingProgress` değerine (Yüzde 0 ile Yüzde 100 arası) göre çatlak dokularından birini hafifçe daha büyük (örneğin 1.01f scale) bir küp olarak çizdirirsin.

Aynı "Cutout" (Discard) hilesini bu çatlak küpü için de kullanırsın, böylece sadece siyah çatlak çizgileri gerçek bloğun üzerine yapışmış gibi görünür.


# 28. soru 

Blok kırıldığında doğrudan envantere ışınlanması yerine, dünyanın içine 3D küçük ve dönen bir obje olarak düşmesini (Item Drop) ve yanına gidince toplanmasını nasıl yapabilirim?

# 28. cevap

Oyun dünyasında fiziksel olarak var olan, yere düşen, kendi etrafında dönen ve yavaşça süzülen eşyalar (Item Drops), oyunun "dokunulabilirlik" (tactile) hissini zirveye çıkaran en önemli detaylardan biridir.

Eğer bloğu kırdığımızda eşyayı doğrudan envantere eklersen, eşya aniden yok olmuş gibi hissettirir. Bunun yerine, kırma işlemi bittiğinde bloğun merkezinde yeni bir **Varlık (Entity)** oluşturacağız.

İşte Minecraft tarzı dönen ve zıplayan eşyaların C++ ile yazılmış anatomisi:

## 1. Düşen Eşya (DroppedItem) Sınıfını Oluşturmak

Daha önce Tavuk ve Zombi için kurduğumuz `Entity` sistemini kullanarak yeni bir canlı türü üretiyoruz: `DroppedItem`. Bu varlığın yapay zekası (AI) oyuncuya saldırmak değil, yerçekimiyle yere düşmek ve oyuncu yaklaşınca envantere girmektir.

```cpp
class DroppedItem : public Entity {
public:
    int itemID;
    int count;
    
    float rotationAngle = 0.0f; // Kendi etrafında dönme açısı
    float hoverTime = 0.0f;     // Yukarı aşağı süzülme animasyonu için
    float pickupDelay = 1.0f;   // Kırıldığı an oyuncuya yapışmaması için 1 saniyelik bekleme süresi

    DroppedItem(glm::vec3 startPos, int id, int amount) {
        // Bloğun tam ortasından başlasın
        position = startPos + glm::vec3(0.5f, 0.5f, 0.5f); 
        itemID = id;
        count = amount;
        
        // Kutumuz zombilerden çok daha küçük
        width = 0.25f;
        height = 0.25f;

        // FİZİK: Eşya ilk çıktığında rastgele bir yöne doğru hafifçe fırlasın ("Pop" efekti)
        float randX = ((rand() % 100) / 100.0f) * 2.0f - 1.0f; // -1.0 ile 1.0 arası
        float randZ = ((rand() % 100) / 100.0f) * 2.0f - 1.0f;
        
        velocity = glm::vec3(randX * 1.5f, 3.5f, randZ * 1.5f); // Yukarı doğru fırlat
    }

    void UpdateAI(float deltaTime, Player& player, World& world) override {
        // 1. ANİMASYON (Dönme ve Süzülme)
        rotationAngle += deltaTime * 2.0f; // Saniyede ~114 derece döner
        hoverTime += deltaTime * 3.0f;

        // 2. FİZİK VE YERÇEKİMİ
        velocity.y -= 20.0f * deltaTime; // Yerçekimi
        
        // Eşya fiziğini uygula (Zombilerde kullandığımız Move/Çarpışma fonksiyonunun aynısı)
        // Eğer yere çarparsa velocity.x ve velocity.z'yi sıfırlayarak kaymasını durdurmalısın (Sürtünme).
        Move(velocity * deltaTime, world);

        // 3. TOPLANMA MANTIĞI (Pick-up)
        if (pickupDelay > 0.0f) {
            pickupDelay -= deltaTime;
        } 
        else {
            // Oyuncu ile eşya arasındaki mesafeyi ölç
            float distance = glm::length(player.Position - position);
            
            // Eğer oyuncu 1.5 blok yakındaysa (Manyetik Çekim / Toplama)
            if (distance < 1.5f) {
                // Eşyayı envantere eklemeye çalış
                bool pickedUp = player.GiveItem(itemID, count);
                
                if (pickedUp) {
                    isDead = true; // Eşyayı dünyadan sil
                    // Ses çal: audio.Play2DSound("assets/sounds/pop.wav");
                }
            }
        }
    }
    
    // OnDeath fonksiyonu boş kalabilir (Çünkü oyuncuya hasar veremez)
    void OnDeath(Player& player) override {}
};

```

---

## 2. Kırılan Bloğu Eşyaya Dönüştürmek (Spawning)

Bir önceki adımda yazdığımız basılı tutarak kırma (`UpdateMining`) fonksiyonunda, bloğu dünyadan sildiğimiz (`SetBlock(..., AIR)`) satırın hemen altına bu eşyayı dünyadaki canlılar listesine ekleme kodunu yazıyoruz:

```cpp
// ... kırma süresi doldu (breakingProgress >= 1.5f)
if (breakingProgress >= 1.5f) {
    
    int brokenBlockID = world.GetBlock(targetBlock.x, targetBlock.y, targetBlock.z);
    
    // 1. Bloğu haritadan sil (Hava yap)
    world.SetBlock(targetBlock.x, targetBlock.y, targetBlock.z, AIR);
    
    // 2. DÜNYAYA EŞYA OLARAK FIRLAT
    // activeEntities listene yeni bir DroppedItem ekliyoruz
    activeEntities.push_back(new DroppedItem(targetBlock, brokenBlockID, 1));
    
    // 3. Aletin canını düşür...
}

```

---

## 3. Eşyayı 3D Olarak Çizdirmek (Render)

Sıra geldi bu eşyayı ekranda küçük ve tatlı bir şekilde çizdirmeye. Elimizde zaten Zombiler ve Dünya için kullandığımız 1x1 boyutunda standart bir Küp VAO'su var. Bu küpü alıp **küçülteceğiz, Y ekseninde döndüreceğiz ve hafifçe aşağı yukarı süzülmesini** sağlayacağız.

Dünyadaki varlıkları (Entity) çizdiğin döngüye şu koşulu ekle:

```cpp
void RenderDroppedItem(DroppedItem* item, unsigned int blockAtlasTexture, unsigned int cubeVAO, Shader& shader3D) {
    
    // Eşyanın animasyonlu pozisyonunu hesapla (Sinüs dalgası ile süzülme)
    float hoverOffset = sin(item->hoverTime) * 0.1f; 
    glm::vec3 renderPos = item->position + glm::vec3(0.0f, hoverOffset, 0.0f);

    glm::mat4 model = glm::mat4(1.0f);
    
    // 1. Eşyayı konuma taşı
    model = glm::translate(model, renderPos);
    
    // 2. Kendi etrafında sürekli döndür (Y ekseninde)
    model = glm::rotate(model, item->rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // 3. Orijinal bloğun %25'i boyutuna küçült
    model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

    shader3D.setMat4("model", model);

    // 4. Bloğun dokusunu Atlas'tan bul (Tıpkı arayüzde yaptığımız UV hesaplaması gibi)
    // Not: 3D Shader'ına 'uvOffset' değişkeni ekleyerek tüm küpü atlasın sadece bir kısmıyla kaplayabilirsin.
    float uvSize = 1.0f / 16.0f;
    float uMin = (item->itemID % 16) * uvSize;
    float vMin = (item->itemID / 16) * uvSize;
    
    shader3D.setVec4("uvOffset", glm::vec4(uMin, vMin, uvSize, uvSize));

    // Küpü çiz
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blockAtlasTexture);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

```

> **Görsel Detay:** Eğer yere düşen eşya bir blok değil de sadece envanterde duran 2D bir eşyaysa (Örn: Çiğ Tavuk `RAW_CHICKEN` veya Tüy `FEATHER`), 3D bir küp (`cubeVAO`) yerine 2D düz bir Dörtgen (`quadVAO`) çizebilirsin. Minecraft'taki gibi dörtgenin her zaman kameraya dönük olmasını (Billboard effect) sağlamak için, çizim matrisindeki rotasyon kısmını kameranın kendi rotasyonu (Yaw açısı) ile eşitlemen yeterlidir.

Bu sistemi kurduktan sonra bir bloğu kırdığında, bloğun içinden küçük boyutlu versiyonunun zıplayarak çıktığını, yere düştükten sonra süzülerek kendi etrafında döndüğünü ve sen üzerine doğru yürüyüp 1.5 blok yaklaştığında tatlı bir "pop" sesiyle envanterine uçtuğunu göreceksin!


# 29 . soru

Blok kırıldığında veya zombiye vurduğumda çevreye saçılan o küçük taş/kan kırıklarını (Particle Effects) yüksek performansla ekranda nasıl oluşturabilirim?


# 29 . cevap

Oyunlara o oynaması inanılmaz keyifli, "sulu" (juicy) hissiyatı veren şey kesinlikle parçacık (Particle) sistemleridir. Bir zombiye vurduğunda sıçrayan kanlar (çürük et parçaları) veya taşı kırdığında etrafa saçılan o minik ufalanmış taş kırıkları, beynimize eylemin gerçekleştiğine dair harika bir geri bildirim yollar.

Ancak burada çok kritik bir performans kuralı devreye girer: **1.000 adet parçacık için ekrana 1.000 kez `glDrawArrays` komutu gönderirsen (yani önceki eşyalarda yaptığımız gibi tek tek çizersen) FPS anında yerlerde sürünür.**

Binlerce parçacığı **tek bir çizim çağrısıyla (1 Draw Call)** ve işlemciyi hiç yormadan ekrana basmak için iki harika teknik kullanacağız: **Object Pooling (Nesne Havuzu)** ve **Instanced Rendering (Çoğaltılmış Çizim)**.

Ayrıca bu parçacıkların 3D küpler değil, her zaman kameraya dönük olan 2D düzlemler (Billboard) olmasını sağlayacağız (Minecraft'taki kırma efektleri de aslında 2D'dir).

İşte yüksek performanslı parçacık motorunun kurulumu:

## 1. Nesne Havuzu (Object Pool) Kurmak

Oyun programlamada sürekli `new` ve `delete` (veya `std::vector::push_back`) kullanmak belleği (RAM) parçalar ve takılmalara (Garbage Collection stutter) yol açar. Bunun yerine oyun açıldığında 5.000 adet parçacığı tek seferde oluşturup, sadece "Aktif" ve "Pasif" durumları arasında geçiş yaptıracağız.

```cpp
struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 uvOffset; // Atlas üzerindeki dokusu
    float life;         // Kalan ömrü
    float size;
    bool active;        // Ekranda mı, yoksa havuzda bekliyor mu?
};

class ParticleSystem {
public:
    static const int MAX_PARTICLES = 5000;
    Particle particles[MAX_PARTICLES];
    int lastUsedParticle = 0;

    ParticleSystem() {
        for (int i = 0; i < MAX_PARTICLES; i++) {
            particles[i].active = false;
        }
    }

    // Havuzda kullanılmayan (pasif) ilk parçacığı bulur
    int FindUnusedParticle() {
        for (int i = lastUsedParticle; i < MAX_PARTICLES; i++) {
            if (!particles[i].active) {
                lastUsedParticle = i;
                return i;
            }
        }
        for (int i = 0; i < lastUsedParticle; i++) {
            if (!particles[i].active) {
                lastUsedParticle = i;
                return i;
            }
        }
        return 0; // Hepsi doluysa en baştakinin üzerine yaz
    }
};

```

---

## 2. Parçacıkları Oluşturmak (Tetikleyici)

Bir bloğu kırdığımızda bu sisteme "Bana bu bloğun konumunda 30 adet parçacık ver" diyeceğiz. Sistem havuzdan 30 boş yer bulup onlara rastgele fırlama hızları atayacak.

`ParticleSystem` sınıfına şu fonksiyonu ekle:

```cpp
void SpawnBlockParticles(glm::vec3 blockPos, int blockID, int count) {
    
    // Bloğun Atlas üzerindeki yerini hesapla
    float uvSize = 1.0f / 16.0f;
    float uMin = (blockID % 16) * uvSize;
    float vMin = (blockID / 16) * uvSize;

    for (int i = 0; i < count; i++) {
        int pIndex = FindUnusedParticle();
        Particle& p = particles[pIndex];

        p.active = true;
        // Parçacık bloğun rastgele bir noktasından çıksın
        p.position = blockPos + glm::vec3(
            (rand() % 100) / 100.0f, 
            (rand() % 100) / 100.0f, 
            (rand() % 100) / 100.0f
        );

        // Patlama efekti: Her yöne rastgele fırlat
        p.velocity = glm::vec3(
            ((rand() % 100) / 50.0f) - 1.0f, // -1.0 ile 1.0 arası
            ((rand() % 100) / 50.0f) + 1.5f, // Yukarı doğru ekstra güç
            ((rand() % 100) / 50.0f) - 1.0f
        );

        // Bloğun dokusundan rastgele KÜÇÜK bir kare kes
        float subU = uMin + ((rand() % 4) * (uvSize / 4.0f));
        float subV = vMin + ((rand() % 4) * (uvSize / 4.0f));
        p.uvOffset = glm::vec4(subU, subV, uvSize / 4.0f, uvSize / 4.0f);
        
        p.size = 0.1f + ((rand() % 10) / 100.0f); // Rastgele boyut
        p.life = 1.0f + ((rand() % 10) / 10.0f);  // 1 ile 2 saniye arası ömür
    }
}

```

---

## 3. Parçacık Fiziği (Update)

Oyun döngünde her karede (frame) bu parçacıkların yerçekimiyle düşmesini ve ömürleri bitince havuzda tekrar pasif hale geçmelerini sağlamalısın.

```cpp
void UpdateParticles(float deltaTime, World& world) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle& p = particles[i];
        
        if (p.active) {
            p.life -= deltaTime;
            if (p.life <= 0.0f) {
                p.active = false; // Ömrü bitti, havuzda geri dönüştürülebilir!
                continue;
            }

            // Yerçekimi ve Hareket
            p.velocity.y -= 15.0f * deltaTime;
            p.position += p.velocity * deltaTime;

            // İsteğe Bağlı: Yere Çarpma Kontrolü (Hitbox'ı 0.1 alarak IsColliding ile)
            // Eğer yere çarparsa velocity'yi 0 yapıp yerde kalmasını sağlayabilirsin.
        }
    }
}

```

---

## 4. Ekran Kartına "Instanced" Gönderme ve Billboarding (Sihirli Kısım)

Ekrana 1.000 parçacık çizmek için C++ tarafında bir döngü kurup bin kere ekran kartına veri yollamayacağız. Bunun yerine aktif olan parçacıkların **sadece Pozisyonlarını ve UV'lerini** devasa bir diziye koyup, ekran kartına "Al bu diziyi, şu elimdeki 1x1'lik Dörtgeni (Quad) bu dizideki konumların her birine kopyala" diyeceğiz. Buna **Instanced Rendering** denir.

**C++ Çizim Kodları:**

```cpp
// Bu diziler GPU'ya tek seferde gidecek verileri tutar
std::vector<glm::vec3> particlePositions;
std::vector<glm::vec4> particleUVs;

for (int i = 0; i < MAX_PARTICLES; i++) {
    if (particles[i].active) {
        // Zıplayan animasyon vb. eklenebilir
        particlePositions.push_back(particles[i].position);
        particleUVs.push_back(particles[i].uvOffset);
    }
}

// Shader'ı aktif et
particleShader.use();
particleShader.setMat4("projection", projectionMatrix);
particleShader.setMat4("view", camera.GetViewMatrix());

// Kamera yönlerini GPU'ya yolla (Billboarding için)
particleShader.setVec3("cameraRight", glm::normalize(glm::cross(camera.Front, camera.Up)));
particleShader.setVec3("cameraUp", camera.Up);

// -- BURASI ÖNEMLİ: Verileri Ekran Kartına Gönder --
// Instanced rendering için VBO'ları (Vertex Buffer Object) güncelle
glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
glBufferData(GL_ARRAY_BUFFER, particlePositions.size() * sizeof(glm::vec3), particlePositions.data(), GL_DYNAMIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
glBufferData(GL_ARRAY_BUFFER, particleUVs.size() * sizeof(glm::vec4), particleUVs.data(), GL_DYNAMIC_DRAW);

// Tek bir komutla TÜM aktif parçacıkları çiz! (glDrawArrays değil, glDrawArraysInstanced)
glBindVertexArray(particleVAO);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, blockAtlasTexture);

// 6 köşe (Quad), kaç adet çizileceği (size)
glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particlePositions.size()); 

```

---

## 5. Parçacık Shader'ı (Billboarding Matematiği)

Parçacıkların her zaman kameraya dönük olması için çok zekice bir yöntem vardır. Dörtgenin köşelerini X ve Y ekseninde normal şekilde kaydırmak yerine, kameranın **"Sağ" (Right)** ve **"Yukarı" (Up)** vektörlerini kullanarak kaydırırız. Böylece dörtgen her zaman kameraya paralel (dönük) olur.

**`particle_vertex.glsl`:**

```glsl
#version 330 core
layout (location = 0) in vec2 aQuadPos; // Dörtgenin orijinal köşeleri (Örn: -0.5, 0.5)
layout (location = 1) in vec2 aTexCoord;

// Instanced veriler (Her parçacık için farklıdır)
layout (location = 2) in vec3 aInstancePos; // C++'tan gelen pozisyon
layout (location = 3) in vec4 aInstanceUV;  // C++'tan gelen UV kesiti

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraRight; // Kameranın sağı
uniform vec3 cameraUp;    // Kameranın yukarısı

void main() {
    float particleSize = 0.15; // Boyut

    // BILLBOARDING: Dörtgenin köşelerini dünya eksenlerine göre değil, kamera eksenlerine göre büküyoruz.
    vec3 vertexPosition = aInstancePos 
                        + cameraRight * aQuadPos.x * particleSize
                        + cameraUp * aQuadPos.y * particleSize;

    gl_Position = projection * view * vec4(vertexPosition, 1.0);
    
    // UV Offset hesaplaması (Arayüzde yaptığımızın aynısı)
    TexCoords = (aTexCoord * vec2(aInstanceUV.z, aInstanceUV.w)) + vec2(aInstanceUV.x, aInstanceUV.y);
}

```

### Nasıl Tetikleyeceksin?

* **Blok Kırıldığında:** `SetBlock(..., AIR)` dediğin satırın hemen altına: `particleSys.SpawnBlockParticles(hedefBlokPos, kirilanBlokID, 40);`
* **Zombiye Vurduğunda:** Hasar verme satırının altına: `particleSys.SpawnBlockParticles(zombi.position + vec3(0,1,0), ROTTEN_FLESH, 15);`

Bu sistemi eklediğinde kılıcını zombiye savurduğunda etrafa sıçrayan çürük et parçaları ve kazmayla taşı kırdığında dökülen yüzlerce taş parçası oyunun hissiyatını bambaşka bir seviyeye taşıyacaktır!



# 30. soru

Parçacık sistemimiz çok güçlü çalışıyor. Bu aynı mantığı kullanarak tüm gökyüzünden yavaşça düşen yağmur damlalarını ve biyomuna göre (örneğin dağların zirvesinde) yağan kar efektlerini nasıl yapabilirim?

# 30 . cevap

Hava durumu (Yağmur ve Kar) görsel olarak parçacıklara çok benzese de, oyun motorlarında tamamen farklı bir **Optimizasyon Hilesi** ile çalışırlar.

Eğer gökyüzündeki bulutlardan aşağıya binlerce damla oluşturup ömürleri bitince silseydik, bilgisayar bir süre sonra pes ederdi. Bunun yerine "Kafeste Dönen Fare" (Teleporting/Pooling) hilesini kullanırız: Oyuncunun kafasının etrafında sadece 2.000 adet parçacık oluştururuz. Bir damla yere çarptığında (veya oyuncunun görüş alanından çıktığında) onu silmeyiz; **anında tekrar gökyüzüne ışınlarız!**

İşte biyom destekli ve yüksek performanslı hava durumu motoru:

## 1. Yükseklik Haritası (Height Map) İhtiyacı

Yağmur damlasının yerin altına (mağaralara) yağmasını engellemek için, belirli bir X ve Z koordinatında gökyüzünden aşağı inildiğinde karşılaşılan **ilk katı bloğun Y yüksekliğini** bilmemiz gerekir.

`World` sınıfına bu yüksekliği bulan çok basit bir yardımcı fonksiyon ekle:

```cpp
int GetHighestBlockY(int x, int z) {
    // Gökyüzünden (255) aşağı doğru in, ilk katı bloğu bul
    for (int y = 255; y >= 0; y--) {
        if (GetBlock(x, y, z) != AIR && GetBlock(x, y, z) != OAK_LEAVES) {
            return y;
        }
    }
    return 0; // Hiçlik
}

```

---

## 2. Hava Durumu Parçacık Sistemi

Normal parçacık sisteminden ayrı bir `WeatherSystem` kuruyoruz. Bu sistem, oyuncuyu sürekli bir bulut gibi takip edecek.

```cpp
struct WeatherParticle {
    glm::vec3 position;
    float speed;
    bool isSnow;   // Yağmur mu, Kar mı?
    float offsetX; // Kar tanesinin sağa sola sallanması için rastgele bir başlangıç açısı
};

class WeatherSystem {
public:
    static const int MAX_DROPS = 3000; // Ekranda aynı anda bulunacak damla sayısı
    WeatherParticle drops[MAX_DROPS];
    
    // Yağmur yağıyor mu?
    bool isRaining = false; 

    WeatherSystem() {
        // Oyun başında 3000 damlayı rastgele konumlara yerleştir
        for (int i = 0; i < MAX_DROPS; i++) {
            drops[i].offsetX = (rand() % 100) / 100.0f * (2.0f * glm::pi<float>());
            ResetDrop(i, glm::vec3(0)); // Başlangıçta 0,0'a göre ayarla
        }
    }

    // Damlayı gökyüzüne yeniden ışınlayan fonksiyon
    void ResetDrop(int index, glm::vec3 playerPos) {
        // Oyuncunun etrafında 20 blokluk bir yarıçapta rastgele bir X ve Z seç
        float randX = playerPos.x + ((rand() % 40) - 20.0f);
        float randZ = playerPos.z + ((rand() % 40) - 20.0f);
        
        // Gökyüzünde, oyuncunun kafasının 10 ila 25 blok üzerinde rastgele bir Y seç
        float randY = playerPos.y + 10.0f + (rand() % 15);
        
        drops[index].position = glm::vec3(randX, randY, randZ);
        
        // BİYOM / YÜKSEKLİK KONTROLÜ (Kar mı Yağmur mu?)
        // Eğer bu koordinattaki zemin çok yüksekteyse (Dağ zirvesi) kar yağsın!
        // Not: İstersen World'den Sıcaklık (Temperature) verisini de okuyabilirsin
        int groundY = world.GetHighestBlockY(std::floor(randX), std::floor(randZ));
        
        if (groundY > 90) { // 90. bloğun üzeri karlı olsun
            drops[index].isSnow = true;
            drops[index].speed = 3.0f + ((rand() % 10) / 10.0f); // Kar yavaş düşer (3-4 hız)
        } else {
            drops[index].isSnow = false;
            drops[index].speed = 15.0f + ((rand() % 20) / 10.0f); // Yağmur çok hızlı düşer (15-17 hız)
        }
    }

```

---

## 3. Yağmur ve Kar Fiziği (Update Loop)

Zaman aktıkça yağmur damlaları yere doğru bir çizgi halinde iner. Kar taneleri ise rüzgarda sürükleniyormuş gibi Sinüs (`sin`) dalgasıyla sağa sola sallanır.

```cpp
    void Update(float deltaTime, glm::vec3 playerPos, World& world) {
        if (!isRaining) return;

        for (int i = 0; i < MAX_DROPS; i++) {
            WeatherParticle& drop = drops[i];

            // 1. Düşme Fiziği
            drop.position.y -= drop.speed * deltaTime;

            // Eğer kar ise, X ve Z ekseninde rüzgarla hafifçe sallansın
            if (drop.isSnow) {
                // offsetX her kar tanesinin farklı yöne sallanmasını sağlar
                drop.position.x += sin(drop.position.y + drop.offsetX) * deltaTime * 1.5f; 
            }

            // 2. Yere Çarpma Kontrolü
            int blockX = std::floor(drop.position.x);
            int blockZ = std::floor(drop.position.z);
            int groundY = world.GetHighestBlockY(blockX, blockZ);

            // Eğer damla yere çarptıysa VEYA oyuncunun çok altına düştüyse
            if (drop.position.y <= groundY + 0.5f || drop.position.y < playerPos.y - 15.0f) {
                
                // İSTEĞE BAĞLI: Eğer yağmursa, yere çarptığı an az önce yazdığımız 
                // ParticleSystem'i kullanarak 1-2 adet mavi "su sıçrama" parçacığı üretebilirsin!
                
                // Damlayı tekrar gökyüzüne ışınla! (Sıfır performans kaybı)
                ResetDrop(i, playerPos);
            }
        }
    }
};

```

---

## 4. Ekran Kartına Gönderme ve Şekillendirme (Render)

Hava durumu parçacıklarını çizerken daha önce yazdığımız `glDrawArraysInstanced` fonksiyonunu kullanacağız. Ancak yağmur ile karın şekli farklıdır. Yağmur uzun dikey bir çizgidir, kar ise kare şeklindedir.

Bunu çözmek için Instanced verilerimize (Position ve UV yanına) bir de `isSnow` bilgisi (float olarak 0.0 veya 1.0) göndeririz.

**Shader'daki Büyü (`weather_vertex.glsl`):**

```glsl
#version 330 core
layout (location = 0) in vec2 aQuadPos;
layout (location = 1) in vec2 aTexCoord;

layout (location = 2) in vec3 aInstancePos; // Damlanın konumu
layout (location = 3) in float aIsSnow;     // 1.0 = Kar, 0.0 = Yağmur

// ... (Kamera vektörleri vs.)

void main() {
    
    // Boyutlandırma Matematiği
    float width = 0.05;
    float height = 0.05;

    if (aIsSnow < 0.5) {
        // EĞER YAĞMURSA: Genişliği iyice incelt, yüksekliği uzat (Çizgi gibi görünsün)
        width = 0.02;
        height = 0.4;
    } else {
        // EĞER KARSA: Daha büyük ve kare kalsın
        width = 0.08;
        height = 0.08;
    }

    vec3 vertexPosition = aInstancePos 
                        + cameraRight * aQuadPos.x * width
                        + vec3(0.0, 1.0, 0.0) * aQuadPos.y * height; 
                        // Not: Yağmurun kamera yukarı baktığında bükülmemesi için 
                        // Y eksenini sabit (0,1,0) olarak kilitliyoruz.

    gl_Position = projection * view * vec4(vertexPosition, 1.0);
    // ...
}

```

Bu sistemi kurup `/weather rain` gibi bir komutla aktif ettiğinde; dağların zirvesinde yürürken üzerine lapa lapa kar yağdığını, dağdan aşağı ormanlık biyoma indiğinde ise bu karların anında süzülen yağmur damlalarına dönüştüğünü göreceksin!

# 31. soru

Oyun motorunun çekirdek mekaniklerini, görselliğini ve evrenini tamamen bitirdik. Bu C++ / OpenGL motorunu çapraz platform (Cross-Platform) mimarisiyle telefonlara taşımak ve ekrana sanal joystick (Touch Controls) eklemek için CMake ve NDK ile nasıl bir yol izlemeliyim?

# 31. cevap

Masaüstünde çalışan, sıfırdan yazılmış bir C++ motorunu Android gibi mobil bir platforma taşımak (Porting), projenin en tatmin edici aşamalarından biridir. Android'in yerel (Native) katmanında çalışacak bu motor, cihazın tüm donanım gücünü doğrudan kullanacaktır.

Şu ana kadar penceremizi ve girdilerimizi yönetmek için **SDL2** kullandık. Bu bizim en büyük avantajımız; çünkü SDL2, Android işletim sistemiyle C++ kodlarımız arasında mükemmel bir köprü (Wrapper) kurar. Java/Kotlin ile neredeyse hiç uğraşmadan, doğrudan NDK (Native Development Kit) üzerinden C++ ile devam edeceğiz.

İşte motorunu Android'e taşımanın 4 temel adımı:

### 1. OpenGL'den OpenGL ES'e Geçiş (GLES 3.0)

Mobil cihazlar, masaüstü ekran kartları gibi tam sürüm OpenGL desteklemez. Bunun yerine gömülü sistemler için optimize edilmiş **OpenGL ES (Embedded Systems)** kullanırlar. Motorumuz zaten modern OpenGL (3.3) altyapısında olduğu için geçiş çok kolaydır.

Shader dosyalarını (`.glsl`) açıp şu iki değişikliği yapmalısın:

1. En üstteki versiyonu değiştir: `#version 330 core` yerine `#version 300 es` yaz.
2. Fragment (Renk) shader'larına hassasiyet (Precision) kuralı ekle. Mobil GPU'lar pil tasarrufu için değişken boyutlarını bilmek ister:
```glsl
#version 300 es
precision mediump float; // Tüm float'lar orta hassasiyette hesaplansın
// ... geri kalan kodlar aynı ...

```



### 2. CMake ve NDK Yapılandırması

Android için derleme yaparken sistemindeki standart GCC/Clang yerine NDK içindeki çapraz derleyiciyi (Cross-compiler) kullanman gerekir. CMakeLists.txt dosyanı, Android NDK'nın `android.toolchain.cmake` dosyasıyla besleyeceğiz.

Android platformunda `main` fonksiyonun doğrudan çalışmaz. SDL2, C++ kodunu bir paylaşımlı kütüphane (`.so` dosyası) olarak derler ve Android'in `SDLActivity.java` sınıfı JNI (Java Native Interface) üzerinden senin `main` fonksiyonunu çağırır.

CMakeLists.txt dosyana şu kısımları eklemelisin:

```cmake
# Eğer Android için derleniyorsa
if(ANDROID)
    # SDL2, Android için kendi main köprüsünü kurar
    add_library(OyunMotoru SHARED src/main.cpp src/glad.c ...) 
    
    # Android'in yerel kütüphanelerini (Log, GLES3) projeye bağla
    target_link_libraries(OyunMotoru 
        SDL2 
        GLESv3 
        log 
        android
    )
else()
    # Masaüstü derleme ayarları (eski kodların)
    add_executable(OyunMotoru src/main.cpp ...)
endif()

```

### 3. Çoklu Dokunmatik Sistem (Multitouch)

Mobilde klavye ve fare yok. Bunun yerine oyuncunun baş parmaklarını kullanacağız: **Ekranın sol yarısı Joystick (Yürüme), sağ yarısı ise Kamera (Etrafı izleme) ve Kırma/Koyma olacak.**

SDL2, dokunmatik ekranları `SDL_TouchFingerEvent` ile yakalar. Mobilde parmak koordinatları piksel olarak değil, **0.0 ile 1.0 arasında normalize edilmiş** olarak gelir.

`main.cpp` içindeki Girdi (Input) döngünü şu şekilde güncelle:

```cpp
// Parmakların ekrana ilk dokunduğu yerleri hafızada tutalım
glm::vec2 leftFingerStart(-1.0f); 
int leftFingerID = -1; // Hangi parmak joystick'i kullanıyor?

glm::vec2 rightFingerStart(-1.0f);
int rightFingerID = -1;

SDL_Event e;
while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) running = false;

    // PARMAK EKRANA DOKUNDU
    if (e.type == SDL_FINGERDOWN) {
        float touchX = e.tfinger.x;
        float touchY = e.tfinger.y;
        
        if (touchX < 0.5f && leftFingerID == -1) {
            // Sol tarafa dokunuldu -> Joystick başlat
            leftFingerID = e.tfinger.fingerId;
            leftFingerStart = glm::vec2(touchX, touchY);
        } 
        else if (touchX >= 0.5f && rightFingerID == -1) {
            // Sağ tarafa dokunuldu -> Kamera başlat
            rightFingerID = e.tfinger.fingerId;
            rightFingerStart = glm::vec2(touchX, touchY);
        }
    }

    // PARMAK EKRANDA KAYDIRILDI
    else if (e.type == SDL_FINGERMOTION) {
        if (e.tfinger.fingerId == leftFingerID) {
            // Sanal Joystick Hareketi
            float deltaX = e.tfinger.x - leftFingerStart.x;
            float deltaY = e.tfinger.y - leftFingerStart.y;
            
            // deltaX ve deltaY'yi oyuncunun Velocity'sine (Yürüme hızı) aktar
            // Eksi Y ileri (W tuşu), Artı Y geri (S tuşu) gibi düşünülebilir.
            player.SetJoystickInput(deltaX, deltaY);
        }
        else if (e.tfinger.fingerId == rightFingerID) {
            // Kamera Dönüşü (Fare kaydırmanın mobildeki karşılığı)
            float lookDeltaX = e.tfinger.dx * 1000.0f; // dx: Bir önceki kareye göre değişim
            float lookDeltaY = e.tfinger.dy * 1000.0f;
            
            camera.ProcessMouseMovement(lookDeltaX, -lookDeltaY);
        }
    }

    // PARMAK EKRANDAN ÇEKİLDİ
    else if (e.type == SDL_FINGERUP) {
        if (e.tfinger.fingerId == leftFingerID) {
            leftFingerID = -1;
            player.SetJoystickInput(0, 0); // Karakteri durdur
        }
        else if (e.tfinger.fingerId == rightFingerID) {
            rightFingerID = -1;
            
            // Eğer parmak çok kısa süre durduysa (Tap/Tıklama), blok kır/koy işlemi yap
            // (Buraya bir Timer ekleyerek "Basılı tutma" ile "Tıklama" ayrımı yapabilirsin)
        }
    }
}

```

### 4. Sanal Joystick'i Ekrana Çizmek (UI)

Görsel geri bildirim olmadan dokunmatik kontroller boşlukta hissettirir. Sol başparmak ekrana dokunduğu anda (`FINGERDOWN`), o koordinata saydam bir daire (D-Pad arka planı) çizmeli ve parmak kaydıkça dairenin merkezindeki küçük topu parmak yönünde hareket ettirmelisin.

Bunu, arayüz (UI) çizerken kullandığımız Dörtgen (Quad) ve `DrawUIElement` fonksiyonuyla, alfa değeri düşük (şeffaf) bir doku haritası (joy_bg.png ve joy_knob.png) kullanarak `Render` döngünün en sonunda ekrana basabilirsin.

# 32. soru

Uygulamayı APK olarak derlediğimizde, oyunun içindeki doku (PNG), ses ve Shader dosyalarını C++ tarafında Android'in paketlenmiş yapısından (AAssetManager) nasıl okuyabilirim?

# 32. cevap

Masaüstünden mobile geçişte geliştiricilerin büyük çoğunluğunun duvara tosladığı o kritik noktaya geldin. Windows veya Linux'ta klasörde duran dosyalar (`assets/texture.png`), Android'de derleme sonrasında `.apk` uzantılı bir ZIP arşivinin içine sıkıştırılır. Bu yüzden standart C++ kütüphaneleri (`std::ifstream` veya `fopen`) bu dosyaları bulamaz ve oyun siyah ekranda çöker.

Bunu çözmek için Android NDK'nın sunduğu `AAssetManager` arayüzünü kullanarak dosyaları diskten okumak yerine, **doğrudan belleğe (RAM'e) bir Byte dizisi (Buffer) olarak** almalıyız.

### 1. AAssetManager ile Belleğe Okuma

Android kütüphanelerini (`<android/asset_manager.h>`) projene dahil ettikten sonra, bir dosyayı RAM'e çeken evrensel bir okuma fonksiyonu yazabilirsin:

```cpp
#include <android/asset_manager.h>
#include <vector>

// Bu fonksiyon dosyanın ham baytlarını bir vektör olarak döndürür
std::vector<unsigned char> ReadAssetToMemory(AAssetManager* assetManager, const char* filename) {
    std::vector<unsigned char> buffer;
    
    // AssetManager'dan dosyayı bul ve aç
    AAsset* asset = AAssetManager_open(assetManager, filename, AASSET_MODE_BUFFER);
    if (asset != nullptr) {
        // Dosyanın boyutunu öğren
        off_t length = AAsset_getLength(asset);
        buffer.resize(length);
        
        // Tüm veriyi vektörün içine kopyala
        AAsset_read(asset, buffer.data(), length);
        
        // İşin bitince dosyayı kapat
        AAsset_close(asset);
    }
    
    return buffer;
}

```

### 2. Kütüphaneleri Bellekten Beslemek (stb_image)

Artık elimizde bir dosya yolu değil, RAM'de duran saf bir veri var. Kullandığımız harici kütüphaneleri "dosyadan oku" modundan "bellekten oku" moduna geçirmeliyiz.

Örneğin, `stb_image` ile PNG okurken `stbi_load` yerine **`stbi_load_from_memory`** kullanmalısın:

```cpp
std::vector<unsigned char> imgData = ReadAssetToMemory(assetManager, "textures/atlas.png");

int width, height, channels;
// Veriyi doğrudan RAM'den çöz
unsigned char* image = stbi_load_from_memory(imgData.data(), imgData.size(), &width, &height, &channels, 0);

// ... OpenGL glTexture2D işlemlerini yap ve resmi sil (stbi_image_free)

```

Aynı mantık `miniaudio` için de geçerlidir; sesleri dosya yolundan değil, hafızaya aldığın buffer üzerinden `ma_decoder_init_memory` ile başlatırsın.

### 3. Shader ve Metin Dosyalarını Okumak

Gölge (Shader) dosyaların `.glsl` aslında düz metin dosyalarıdır. Byte dizisini string'e çevirmek için sonuna bir bitiş karakteri (`\0`) eklemek yeterlidir:

```cpp
std::vector<unsigned char> shaderData = ReadAssetToMemory(assetManager, "shaders/vertex.glsl");

// Byte dizisini C++ String'e çevir
std::string shaderCode(shaderData.begin(), shaderData.end());

// Artık shaderCode.c_str() ile OpenGL'e gönderebilirsin

```

### 4. SDL2'nin Hayat Kurtaran Kısayolu

Normal şartlarda `AAssetManager` referansını Java (JNI) katmanından C++'a taşımak oldukça zahmetlidir. Ancak motorun temelinde SDL2 kullandığımız için çok şanslısın! SDL2, Android'in APK paketleme sistemini arkada kendi başına halleder ve sana `SDL_RWops` adında evrensel bir köprü sunar.

Hiç NDK kodlarıyla boğuşmadan, SDL2'nin dahili fonksiyonuyla dosyayı doğrudan APK içinden okuyabilirsin:

```cpp
// SDL2 arka planda otomatik olarak AAssetManager kullanır
SDL_RWops* file = SDL_RWFromFile("textures/atlas.png", "rb");
if (file) {
    Sint64 size = SDL_RWsize(file);
    std::vector<unsigned char> buffer(size);
    
    SDL_RWread(file, buffer.data(), 1, size);
    SDL_RWclose(file);
    
    // Buffer'ı stb_image'e ver...
}

```

Android derlemesini ve donanım entegrasyonunu da başarıyla tamamladığımıza göre, bu Voxel motorunu bir adım daha ileri taşıyıp arkadaşlarımızla aynı haritada oynamak için Çok Oyunculu (Multiplayer / Client-Server) ağ altyapısını kurmaya ne dersin?

# 33. Soru 

evet lütfen Çok Oyunculu (Multiplayer / Client-Server) ağ altyapısını kuralım.

# 33 . cevap

Oyun motorunu tek oyunculudan çok oyunculuya (Multiplayer) geçirmek, bir geliştiricinin atabileceği en büyük ve en prestijli adımdır. Bu noktadan sonra oyunun sadece senin bilgisayarında (veya telefonunda) çalışan bir program olmaktan çıkıp, insanların içinde buluştuğu gerçek bir "Evren" haline gelir.

Voxel tabanlı oyunlarda (Minecraft gibi) ağ programlaması yaparken TCP yerine **UDP** protokolü kullanılır. Çünkü oyuncu hareketlerini saniyede 20 kez gönderirken yolda kaybolan bir paketin tekrar gönderilmesini beklemek oyunu dondurur. Ancak Chunk verileri veya kırılan bloklar kesinlikle kaybolmamalıdır.

Hem UDP'nin hızını hem de TCP'nin güvenilirliğini aynı anda sunan, sektör standardı ve C++ projelerine entegresi en kolay kütüphane **ENet**'tir.

İşte motoruna İstemci-Sunucu (Client-Server) mimarisini kurmanın adımları:

## 1. Otoriter Sunucu Mimarisi (Authoritative Server)

Çok oyunculu oyunların altın kuralı şudur: **İstemciye (Oyuncuya) asla güvenme!**
Eğer oyuncunun bilgisayarı "Ben elmas buldum" derse ve bunu diğerlerine yayarsa, hileciler oyunu saniyeler içinde mahveder.

* **Sunucu (Server):** Dünyanın, Chunk'ların ve diğer oyuncuların nerede olduğunu bilen tek otoritedir. Grafik (OpenGL) çizmez, sadece verileri hesaplar.
* **İstemci (Client):** Sadece oyuncunun klavye/dokunmatik girdilerini sunucuya gönderir ve sunucudan gelen verilere göre ekran kartına çizim emri verir.

## 2. Paket Yapılarını (Protokol) Tanımlamak

Sunucu ve istemcinin birbiriyle konuşabilmesi için ortak bir dile (Paket tiplerine) ihtiyacı vardır. Projende ağ (Network) kütüphanen için ortak bir başlık dosyası oluştur:

```cpp
// NetworkPackets.h
enum PacketType {
    PACKET_CONNECT = 0,     // Sunucuya bağlanma isteği
    PACKET_SPAWN_PLAYER,    // Dünyaya yeni biri katıldı
    PACKET_PLAYER_POS,      // X, Y, Z konum güncellemeleri
    PACKET_BLOCK_BREAK,     // Oyuncu bir bloğu kırdı
    PACKET_BLOCK_PLACE,     // Oyuncu bir blok koydu
    PACKET_CHUNK_DATA       // Sunucudan oyuncuya gönderilen harita verisi
};

// Ağ üzerinden gönderilecek verinin yapısı (Örnek: Oyuncu Hareketi)
struct PacketPlayerPos {
    PacketType type = PACKET_PLAYER_POS;
    int playerID;
    float x, y, z;
    float yaw, pitch;
};

// Örnek: Blok Kırma İsteği
struct PacketBlockBreak {
    PacketType type = PACKET_BLOCK_BREAK;
    int blockX, blockY, blockZ;
};

```

## 3. Sunucuyu (Server) Başlatmak

Sunucu, belirli bir portu (Örn: 7777) dinleyen ve sonsuz bir döngüde dönen konsol tabanlı bir uygulamadır.

```cpp
#include <enet/enet.h>
#include <iostream>

void StartServer() {
    if (enet_initialize() != 0) {
        std::cout << "ENet baslatilamadi!" << std::endl;
        return;
    }

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 7777;

    // Maksimum 32 oyuncu, 2 kanal (0: Güvenilir, 1: Hızlı)
    ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);

    ENetEvent event;
    while (true) { // Sunucu Döngüsü
        
        // Ağda yeni bir olay var mı bekle/oku (Saniyede 20 kez = 50ms Tick Rate)
        while (enet_host_service(server, &event, 50) > 0) {
            
            if (event.type == ENET_EVENT_TYPE_CONNECT) {
                std::cout << "Yeni bir oyuncu baglandi!" << std::endl;
                
                // Oyuncuya etrafındaki Chunk'ları (PACKET_CHUNK_DATA) gönder
            } 
            else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                // Gelen verinin ilk Byte'ını okuyarak paket tipini anla
                PacketType* type = (PacketType*)event.packet->data;
                
                if (*type == PACKET_BLOCK_BREAK) {
                    PacketBlockBreak* packet = (PacketBlockBreak*)event.packet->data;
                    
                    // 1. Sunucu haritasından bloğu sil (AIR yap)
                    // world.SetBlock(packet->blockX, packet->blockY, packet->blockZ, AIR);
                    
                    // 2. Bu değişikliği ağdaki DİĞER TÜM oyunculara bildir (Broadcast)
                    enet_host_broadcast(server, 0, event.packet);
                }
                
                // Paketi bellekten temizle
                enet_packet_destroy(event.packet);
            } 
            else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                std::cout << "Oyuncu ayrildi." << std::endl;
            }
        }
        
        // Sunucu dünyasındaki yapay zekaları (Tavuk, Zombi) hareket ettir
        // ...
    }
}

```

## 4. İstemci (Oyun) Tarafı ve Enterpolasyon (Yumuşatma)

Oyuncunun telefonunda veya bilgisayarında çalışan istemci, sunucuya bağlanır ve sürekli kendi konumunu gönderir.

```cpp
ENetHost* client = enet_host_create(NULL, 1, 2, 0, 0);
ENetAddress address;
enet_address_set_host(&address, "127.0.0.1"); // Sunucu IP'si
address.port = 7777;

ENetPeer* peer = enet_host_connect(client, &address, 2, 0);

```

**Sihirli Kısım: İnterpolasyon (Yumuşatma)**
Ağ verileri saniyede 20 kez (20 TPS) gelir, ancak oyunun 60 FPS çalışır. Eğer diğer oyuncuları sadece sunucudan gelen konumlara anında eşitlersen, diğer oyuncular ekranda yürüyormuş gibi değil, ışınlanıyormuş (titreyerek) gibi görünür.

Bunu çözmek için diğer oyuncuları (RemotePlayers) çizerken, gelen son hedef konuma doğrudan ışınlamak yerine, `glm::mix` (veya Lerp) kullanarak o konuma doğru yavaşça kaydırırız:

```cpp
void UpdateRemotePlayers(float deltaTime) {
    for (auto& remotePlayer : remotePlayersList) {
        
        // Sunucudan gelen gercek hedef konum
        glm::vec3 targetPos = remotePlayer.serverTargetPosition; 
        
        // Karakteri hedef konuma dogru yumusakca kaydir (Interpolation)
        remotePlayer.currentRenderPos = glm::mix(remotePlayer.currentRenderPos, targetPos, 15.0f * deltaTime);
        
        // remotePlayer.currentRenderPos koordinatına zombi çizer gibi bir 3D karakter (Steve) çiz!
    }
}

```

## 5. Blok Kırma İsteğini Göndermek (İstemci)

Oyuncu sol tıka basıp `breakingProgress` dolduğunda, bloğu doğrudan kendi dünyasından (World) silmez. Bunun yerine sunucuya "Ben bu koordinatı kazdım" der.

```cpp
// İstemci tarafındaki blok kırma kodu
PacketBlockBreak breakPacket;
breakPacket.type = PACKET_BLOCK_BREAK;
breakPacket.blockX = targetBlock.x;
breakPacket.blockY = targetBlock.y;
breakPacket.blockZ = targetBlock.z;

// ENet üzerinden güvenilir (ENET_PACKET_FLAG_RELIABLE) olarak gönder
ENetPacket* packet = enet_packet_create(&breakPacket, sizeof(PacketBlockBreak), ENET_PACKET_FLAG_RELIABLE);
enet_peer_send(peer, 0, packet);

```

Sunucu bu paketi onaylayıp tüm oyunculara geri gönderdiğinde, senin istemcin de dahil herkes o bloğun silindiğini görür ve ekran kartlarındaki Mesh'leri (Chunk yapılarını) günceller.


# 34. soru

Sunucudan onay beklerken blokları kırma anında bir gecikme (Ping) hissi oluşuyor. Bu bekleme süresini gizlemek için profesyonel oyunların kullandığı 'İstemci Tarafı Tahminleme' (Client-Side Prediction) hilesini nasıl uygulayabilirim?


# 34 . cevap

Ağ programlamasında tam olarak en can alıcı noktaya parmak bastın! Eğer oyuncu bir bloğu kırar, paket sunucuya gider, sunucu onaylar ve paket geri dönene kadar (örneğin 150ms Ping süresince) ekrandaki blok yerinde durursa, oyun oyuncuya "ağır, hantal ve bozuk" hissettirir.

Profesyonel çok oyunculu oyunların (CS:GO, Minecraft, Valorant) bu gecikmeyi gizlemek için kullandığı sihirli hilenin adı **İstemci Tarafı Tahminleme (Client-Side Prediction)** ve **Sunucu Uzlaştırmasıdır (Server Reconciliation)**.

Bunun temel felsefesi şudur: **"Önce yap ve oyuncuya göster, sonra sunucudan izin iste. Eğer sunucu reddederse, çaktırmadan geri al (Rollback)."**

İşte bu kusursuz hissi motoruna eklemenin adımları:

### 1. Görsel İllüzyon: Anında Tepki (Client-Side)

İstemci tarafında (oyuncunun kendi bilgisayarında/telefonunda) sol tıklama süresi dolduğunda, sunucuyu **beklemeden** bloğu hemen dünyadan silmeli, kırma sesini çalmalı ve parçacıkları ekrana saçmalıyız.

Önceki adımlarda yazdığımız İstemci tarafı `UpdateMining` kodunu şu şekilde güncelliyoruz:

```cpp
if (breakingProgress >= 1.5f) { // Kırma süresi doldu
    
    int brokenBlockID = world.GetBlock(targetBlock.x, targetBlock.y, targetBlock.z);
    
    // 1. TAHMİNLEME: Bloğu anında sil ve grafikleri güncelle (Sıfır Gecikme!)
    world.SetBlock(targetBlock.x, targetBlock.y, targetBlock.z, AIR);
    
    // 2. GÖRSEL/İŞİTSEL TEPKİ: Oyuncunun beyni eylemin gerçekleştiğine ikna olur
    particleSys.SpawnBlockParticles(targetBlock, brokenBlockID, 30);
    audio.Play3DSound("assets/sounds/break_stone.wav", targetBlock);
    
    // 3. SUNUCUYA BİLDİR: "Ben bu bloğu kırdım, haberin olsun"
    PacketBlockBreak breakPacket;
    breakPacket.type = PACKET_BLOCK_BREAK;
    breakPacket.blockX = targetBlock.x;
    breakPacket.blockY = targetBlock.y;
    breakPacket.blockZ = targetBlock.z;
    breakPacket.expectedBlockID = brokenBlockID; // Ne kırdığımızı da gönderelim
    
    ENetPacket* packet = enet_packet_create(&breakPacket, sizeof(PacketBlockBreak), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    
    // Kırma sayacını sıfırla...
}

```

### 2. Otoritenin Kararı: Sunucu Doğrulaması (Server-Side)

Sunucu bu paketi aldığında doğrudan kabul etmez. Hilecileri (uzaktan blok kıranlar) ve bağlantı gecikmesinden doğan hataları (örneğin o bloğu senden 0.1 saniye önce başka bir oyuncunun kırmış olması) denetler.

```cpp
// Sunucu Tarafı (ENET_EVENT_TYPE_RECEIVE)
if (*type == PACKET_BLOCK_BREAK) {
    PacketBlockBreak* packet = (PacketBlockBreak*)event.packet->data;
    
    // 1. DOĞRULAMA (Validation)
    float distance = glm::length(player.Position - glm::vec3(packet->blockX, packet->blockY, packet->blockZ));
    int actualBlockID = world.GetBlock(packet->blockX, packet->blockY, packet->blockZ);
    
    // Eğer oyuncu çok uzaktaysa (Hile) VEYA blok zaten kırılmışsa (Gecikme/Çakışma)
    if (distance > 6.0f || actualBlockID != packet->expectedBlockID) {
        
        // REDDET! İstemciye "Senin tahminin yanlış, o blok aslında şu an bu durumda" mesajı yolla.
        PacketBlockUpdate rollbackPacket;
        rollbackPacket.type = PACKET_BLOCK_UPDATE;
        rollbackPacket.blockX = packet->blockX;
        rollbackPacket.blockY = packet->blockY;
        rollbackPacket.blockZ = packet->blockZ;
        rollbackPacket.blockID = actualBlockID; // Sunucudaki GERÇEK blok
        
        SendToClient(player.peer, rollbackPacket); // Sadece o oyuncuya hatasını düzeltmesi için yolla
    } 
    else {
        // KABUL ET!
        world.SetBlock(packet->blockX, packet->blockY, packet->blockZ, AIR);
        
        // Bu değişikliği DİĞER TÜM oyunculara bildir
        // (Kıran oyuncuya göndermene gerek yok, o zaten kendi ekranında çoktan kırdı)
        BroadcastToOthers(player.peer, packet);
        
        // Eşyayı (DroppedItem) dünyada oluştur ve herkese gönder! (Madde 4'te açıklanacak)
        SpawnDroppedItemOnServer(packet->blockX, packet->blockY, packet->blockZ, actualBlockID);
    }
}

```

### 3. Geri Alma: Sunucu Uzlaştırması (Rollback)

Eğer istemcinin (oyuncunun) bağlantısı kötüyse veya hile yapmaya çalıştıysa, sunucu ona bir `PACKET_BLOCK_UPDATE` gönderecektir. İstemci bu paketi aldığında, sunucunun dediğini **sorgusuz sualsiz** uygulamak zorundadır.

```cpp
// İstemci Tarafı Ağ Dinleme Döngüsü
if (packetType == PACKET_BLOCK_UPDATE) {
    PacketBlockUpdate* packet = (PacketBlockUpdate*)event.packet->data;
    
    // Sunucu otoritedir. Eğer biz bloğu kırdıysak ama sunucu "Orada Taş var" diyorsa,
    // o bloğu gözümüzün önünde tekrar Taşa dönüştürürüz (Ghost Block - Hayalet Blok düzelmesi).
    world.SetBlock(packet->blockX, packet->blockY, packet->blockZ, packet->blockID);
}

```

### 4. Altın Kural: Ekonomi ve Fizik Tahmin Edilemez!

İstemci tarafında bloğun kaybolmasını, kırma sesini ve parçacıkları tahmin ettik. **Ancak yere düşen dönen eşyayı (DroppedItem) asla istemcide anında oluşturmamalısın!**

Neden mi? Çünkü eğer istemci kendi başına dünyaya eşya ekleyebilirse, oyunun ekonomisi hacklenir. Bloğu kırdığında parçacıklar hemen uçar (oyuncu tatmin olur), ancak yere düşen elmas (Dropped Item) sunucudan onay geldikten sonra, yani yaklaşık 100-150ms sonra "pırt" diye ortaya çıkar.

İnsan beyni, kırma sesi ve taş parçacıklarına o kadar odaklanır ki, asıl eşyanın 0.15 saniye gecikmeli düştüğünü **asla fark etmez**. İşte profesyonel ağ programcılığının (Netcode) sırrı bu göz yanılmasında yatar.





# 35. soru

Blok kırmadaki gecikmeyi çözdük. Peki oyuncunun yürümesi ve zıplamasındaki gecikmeyi çözmek için 'Hareket Tahminlemesi' (Movement Prediction) ve 'Geri Çekme' (Rubberbanding) mekaniklerini nasıl kurabilirim?


# 35. cevap

Oyun programlamanın en zorlu, en çok matematik ve mantık gerektiren "Kara Büyü" kısmına (Netcode) hoş geldin.

Hareket gecikmesini (Input Lag) blok kırmadaki gibi çözemeyiz. Çünkü oyuncunun her 16 milisaniyede bir (60 FPS) ilerlemesi gerekir. Eğer W tuşuna basıp sunucudan "ilerleyebilirsin" cevabını beklersen karakterin buzda kayıyormuş gibi hisseder, sürekli takılır.

Profesyonel rekabetçi oyunların (CS:GO, Valorant) ve Minecraft'ın kullandığı altın standart yöntem şudur: **İstemci Tarafı Tahminleme (Client-Side Prediction)** ve **Sunucu Uzlaştırması (Server Reconciliation)**.

Bunu başarmak için oyuncunun yaptığı her harekete bir **Sıra Numarası (Sequence Number / Tick)** vereceğiz.

---

### 1. Sıra Numarası ve Geçmiş Belleği (History Buffer)

Sunucuya artık "Ben X:10, Y:5 konumundayım" demeyeceğiz (Bunu dersek hileciler istedikleri yere ışınlanır). Bunun yerine sunucuya "Ben 45. Hareketi yaptım, W tuşuna bastım ve 0.016 saniye sürdü" diyeceğiz.

İstemci tarafında, henüz sunucu tarafından onaylanmamış hareketleri tutacağımız bir "Geçmiş" listesine ihtiyacımız var.

```cpp
struct PlayerInput {
    int sequenceNumber; // Bu hareketin kimliği (Örn: 1, 2, 3...)
    float deltaTime;    // Ne kadar sürdüğü
    bool w, a, s, d, space; 
};

// Ağ paketi yapımız
struct PacketPlayerInput {
    PacketType type = PACKET_PLAYER_INPUT;
    PlayerInput input;
};

// SADECE İSTEMCİDE (CLIENT) BULUNAN GEÇMİŞ BELLEĞİ
std::vector<PlayerInput> pendingInputs; 
int currentSequenceNumber = 0;

```

---

### 2. İstemci (Client): Anında Tahminleme (Prediction)

Oyuncu klavyeye bastığında sunucuyu hiç beklemeden fizikleri anında kendi dünyasında çalıştırır. Sonra bu hareketi sunucuya yollar ve bir kopyasını geçmişe (pendingInputs) kaydeder.

`UpdateFizik` fonksiyonumuzu istemci tarafında şu şekilde güncelliyoruz:

```cpp
void ProcessClientMovement(float deltaTime, World& world) {
    // 1. Girdileri al ve numaralandır
    PlayerInput input;
    input.sequenceNumber = ++currentSequenceNumber;
    input.deltaTime = deltaTime;
    input.w = keyboard.W; input.a = keyboard.A; 
    input.s = keyboard.S; input.d = keyboard.D;
    input.space = keyboard.Space;

    // 2. Anında Fizik Hesaplaması (TAHMİNLEME)
    // Karakteri hemen yürüt (Gecikme sıfır!)
    ApplyPhysics(player, input, world); 

    // 3. Sunucuya Gönder (UDP ile hızlıca)
    PacketPlayerInput packet;
    packet.input = input;
    SendToServer(packet, ENET_PACKET_FLAG_UNSEQUENCED); // Hızlı gitmesi için Reliable kullanılmaz

    // 4. Onay beklemek üzere listeye ekle
    pendingInputs.push_back(input);
}

```

---

### 3. Sunucu (Server): Otoriter Fizik

Sunucu bu paketi aldığında, oyuncunun konumunu doğrudan güncellemez. İstemcinin gönderdiği tuşları (W, A, S, D) kullanarak **Fizik motorunu kendi haritasında da çalıştırır**.

```cpp
// Sunucu Tarafı (ENET_EVENT_TYPE_RECEIVE)
if (*type == PACKET_PLAYER_INPUT) {
    PacketPlayerInput* packet = (PacketPlayerInput*)event.packet->data;
    
    // Sunucudaki oyuncu nesnesini bul
    RemotePlayer& serverPlayer = players[event.peer->connectID];

    // İstemciden gelen tuşlarla SUNUCUDAKİ fizikleri çalıştır!
    ApplyPhysics(serverPlayer, packet->input, serverWorld);

    // Fizik hesaplandıktan sonra ortaya çıkan KESİN ve GERÇEK konumu istemciye geri yolla
    PacketPlayerState statePacket;
    statePacket.type = PACKET_PLAYER_STATE;
    statePacket.lastProcessedSequence = packet->input.sequenceNumber; // "Senin şu numaranı işledim"
    statePacket.trueX = serverPlayer.position.x;
    statePacket.trueY = serverPlayer.position.y;
    statePacket.trueZ = serverPlayer.position.z;

    SendToClient(event.peer, statePacket);
}

```

---

### 4. Geri Çekme ve Uzlaştırma (Server Reconciliation / Rubberbanding)

İşin en can alıcı noktası burasıdır. İstemci, sunucudan `PACKET_PLAYER_STATE` (Kesin Konum) paketini aldığında ne yapacak?

Eğer 50. hareketi yaptık ama sunucu bize 40. hareketin sonucunu gönderdiyse; önce sunucunun onayladığı tüm eski hareketleri çöpe atarız. Sonra sunucunun konumuna ışınlanır, ardından kalan 10 hareketi **hiç çizmeden (arkaplanda, anında)** tekrar hesaplarız. Buna "Yeniden Simüle Etme (Re-simulation)" denir.

```cpp
// İstemci Tarafı Ağ Dinleme Döngüsü
if (packetType == PACKET_PLAYER_STATE) {
    PacketPlayerState* state = (PacketPlayerState*)event.packet->data;
    
    // 1. Sunucunun işlediği hareketleri listeden sil
    pendingInputs.erase(
        std::remove_if(pendingInputs.begin(), pendingInputs.end(), 
            [state](const PlayerInput& input) {
                return input.sequenceNumber <= state->lastProcessedSequence;
            }),
        pendingInputs.end()
    );

    // 2. Karakteri sunucunun KESİN DEDİĞİ konuma eşitle
    glm::vec3 serverPos(state->trueX, state->trueY, state->trueZ);
    
    // Eğer TAHMİN ettiğimiz konum ile SUNUCU konumu arasında hata payı çok düşükse, 
    // hiçbir şey yapmaya gerek yok (Kusursuz Tahmin)
    if (glm::distance(player.Position, serverPos) < 0.05f) {
        return; 
    }

    // --- HATA VARSA (DESYNC / RUBBERBANDING) ---
    // Eğer buraya düştüysek, oyuncu örneğin kendi ekranında kapıdan geçmiş ama 
    // sunucuda kapı kapalıymış. Veya hile yapıp hızlı koşmaya çalışmış.

    // A) Karakteri gerçek konuma "Geri Çek" (Rubberband)
    player.Position = serverPos;

    // B) Henüz sunucuya ulaşmamış / onaylanmamış hareketleri (Örn: 41'den 50'ye kadar)
    // yeni sunucu konumu üzerinden tekrar hesapla!
    for (const PlayerInput& pendingInput : pendingInputs) {
        ApplyPhysics(player, pendingInput, world);
    }
}

```

### Bu Büyü Nasıl Hissettiriyor?

Oyuncu %99 oranında kendi tahminlediği (`ProcessClientMovement`) koordinatlarda yürür, zıplar ve hiçbir Ping hissetmez. Ancak eğer interneti çok kötüleşirse veya duvarın içinden geçmeye çalışırsa, sunucu hatayı fark eder ve `ApplyPhysics` döngüsü onu duvarın arkasındaki gerçek konumuna doğru geri çeker (İşte oyunlardaki meşhur "Rubberband / Lastik gibi geri sekme" efekti budur).

Bu mimariyle, oyuncuların 200ms ping'i olsa bile kendi ekranlarında tamamen akıcı ve "0 ping" ile oyun oynuyormuş gibi hissederler!


# 36. soru

Hareket ve blok kırma tamam. Peki bir oyuncu oyuna ilk katıldığında devasa Chunk verilerini (her biri 65KB) interneti tıkamadan ve hızlıca göndermek için RLE (Run-Length Encoding) veya Delta Compression algoritmalarını nasıl kurgulayabilirim?


# 36. cevap

Bir oyuncu oyuna ilk katıldığında, etrafındaki (örneğin 9x9'luk bir alandaki) 81 adet Chunk'ı sunucudan çekmesi gerekir. Eğer her biri 65 KB olan bu Chunk'ları ham haliyle gönderirsek, oyuncunun yaklaşık 5.2 MB veriyi indirmesi gerekir. Bu da oyuna girerken uzun yükleme ekranlarına ve sunucuda devasa bant genişliği (Bandwidth) maliyetlerine yol açar.

Voxel oyunlarının verisi eşsiz bir doğaya sahiptir: Haritanın üst tarafında binlerce ardışık Hava (AIR) bloğu, alt tarafında ise binlerce ardışık Taş (STONE) bloğu bulunur. İşte bu yüzden ZIP veya LZ4 gibi genel algoritmalar yerine, oyun dünyasının matematiğine en uygun olan **RLE (Run-Length Encoding)** algoritmasını kurgulayacağız.

RLE mantığı çok basittir: `[HAVA, HAVA, HAVA, HAVA, TAŞ, TAŞ]` göndermek yerine `[4x HAVA, 2x TAŞ]` göndeririz. 65.536 byte'lık tamamen havadan oluşan bir gökyüzü Chunk'ı, bu algoritmayla sadece **3 Byte'a** düşer!

### 1. RLE Sıkıştırma Fonksiyonu (Sunucu Tarafı)

Chunk verimiz bellekte ardışık (contiguous) olarak durur. 3 boyutlu `[16][256][16]` dizimizi sanki tek boyutlu dev bir diziymiş (65.536 elemanlı) gibi düz okuyacağız.

Aynı bloktan kaç tane art arda geldiğini saymak için 16-bitlik (maksimum 65.535'e kadar sayabilen) bir sayaç kullanacağız.

```cpp
#include <vector>
#include <cstdint>

std::vector<uint8_t> CompressChunkRLE(uint8_t* chunkBlocks) {
    std::vector<uint8_t> compressedData;
    
    // Toplam blok sayısı (16 * 256 * 16 = 65536)
    int totalBlocks = 65536; 
    
    uint8_t currentBlockID = chunkBlocks[0];
    uint16_t runCount = 1;

    for (int i = 1; i < totalBlocks; i++) {
        // Eğer blok aynıysa ve sayacımız 16-bit sınırını aşmadıysa saymaya devam et
        if (chunkBlocks[i] == currentBlockID && runCount < 65535) {
            runCount++;
        } 
        else {
            // Seri bozuldu (Farklı bir bloğa geçtik) veya sınır doldu.
            // 1. Sayacı (16-bit) iki adet 8-bit'e (Byte) bölüp listeye ekle
            compressedData.push_back((runCount >> 8) & 0xFF); // Üst byte
            compressedData.push_back(runCount & 0xFF);        // Alt byte
            
            // 2. Blok ID'sini (8-bit) ekle
            compressedData.push_back(currentBlockID);
            
            // Yeni seriyi başlat
            currentBlockID = chunkBlocks[i];
            runCount = 1;
        }
    }
    
    // Döngü bittiğinde elde kalan son seriyi de listeye eklemeyi unutma!
    compressedData.push_back((runCount >> 8) & 0xFF);
    compressedData.push_back(runCount & 0xFF);
    compressedData.push_back(currentBlockID);
    
    return compressedData;
}

```

### 2. Ağa Gönderme İşlemi (ENet)

Sıkıştırılmış verinin boyutu artık 65 KB değil, ortalama bir Chunk için **300 Byte ile 2 KB** arasına düşmüş olacaktır (yaklaşık %95 sıkıştırma oranı). Bu veriyi ENet üzerinden güvenli bir şekilde oyuncuya göndeririz.

```cpp
// Sunucuda bir Chunk'ı gönderme hazırlığı
std::vector<uint8_t> compressedChunk = CompressChunkRLE((uint8_t*)chunk->blocks);

// Gönderilecek Paketin Toplam Boyutu: Paket Tipi (4 byte) + Chunk X/Z (8 byte) + Veri
size_t packetSize = sizeof(PacketType) + (sizeof(int) * 2) + compressedChunk.size();
uint8_t* packetData = new uint8_t[packetSize];

int offset = 0;
PacketType type = PACKET_CHUNK_DATA;

// 1. Paket Tipini Yaz
memcpy(packetData + offset, &type, sizeof(PacketType)); offset += sizeof(PacketType);
// 2. Chunk Koordinatlarını (X ve Z) Yaz
memcpy(packetData + offset, &chunk->chunkX, sizeof(int)); offset += sizeof(int);
memcpy(packetData + offset, &chunk->chunkZ, sizeof(int)); offset += sizeof(int);
// 3. Sıkıştırılmış RLE verisini yaz
memcpy(packetData + offset, compressedChunk.data(), compressedChunk.size());

// ENet üzerinden gönder (Harita verisi olduğu için RELIABLE olmalı, yolda kaybolamaz)
ENetPacket* packet = enet_packet_create(packetData, packetSize, ENET_PACKET_FLAG_RELIABLE);
enet_peer_send(playerPeer, 0, packet);

delete[] packetData;

```

### 3. İstemcide Veriyi Çözmek (Decompression)

Oyuncunun bilgisayarı bu paketi aldığında, RLE verisini tekrar 65.536 elemanlı 3 boyutlu dizi formuna (Chunk) açmalıdır.

```cpp
void DecompressChunkRLE(uint8_t* compressedData, size_t compressedSize, Chunk* targetChunk) {
    
    // Chunk dizisini tek boyutluymuş gibi işaret ediyoruz
    uint8_t* rawBlocks = (uint8_t*)targetChunk->blocks;
    
    int blockIndex = 0;
    int dataIndex = 0;
    
    while (dataIndex < compressedSize && blockIndex < 65536) {
        
        // 1. Sayacı iki Byte'tan birleştirerek oku (16-bit)
        uint16_t runCount = (compressedData[dataIndex] << 8) | compressedData[dataIndex + 1];
        dataIndex += 2;
        
        // 2. Blok ID'sini oku
        uint8_t blockID = compressedData[dataIndex];
        dataIndex += 1;
        
        // 3. Verilen sayı kadar bloğu hedef diziye (RAM'e) yaz
        for (uint16_t i = 0; i < runCount; i++) {
            if (blockIndex < 65536) {
                rawBlocks[blockIndex] = blockID;
                blockIndex++;
            }
        }
    }
    
    // Veri açıldıktan sonra dış yüzeyi hesapla (Mesh Generation) ve ekran kartına yolla
    targetChunk->GenerateMesh();
    targetChunk->BufferData();
}

```

### 4. Delta Compression (Anlık Değişimler)

Bir oyuncu haritayı indirdikten sonra, başka bir oyuncu o Chunk'ın içinde bir ağaç kırarsa **tüm Chunk'ı bir daha asla göndermemelisin!** (RLE veya başka bir şeyle olsa bile).

Burada **Delta Compression (Fark Sıkıştırması)** mantığı devreye girer. Biz bunu ağ programlamasının ilk adımında zaten farkında olmadan kurmuştuk: `PACKET_BLOCK_BREAK` ve `PACKET_BLOCK_PLACE`.

Delta sıkıştırmasının temel kuralı şudur:

1. **Tam Yükleme (Full State):** Oyuncu bölgeye ilk girdiğinde harita RLE ile gönderilir.
2. **Delta (Değişim):** O bölge içindeyken olan her şey sadece `(X, Y, Z, Yeni_Blok_ID)` şeklinde 16 byte'lık minik sinyallerle anlık olarak gönderilir.

Bu iki mimari birleştiğinde (RLE ile Hızlı Başlangıç + Delta ile Anlık Güncelleme), sunucun aynı anda yüzlerce oyuncuyu yorulmadan kaldırabilecek profesyonel bir ağ altyapısına kavuşmuş olur.


# 37 . soru

Tüm oyun döngüsü ve ağ altyapısı bitti! Oyunu başlattığımda beni karşılayan, sunucu IP'sini girebileceğim, 'Tek Oyuncu' ve 'Çok Oyuncu' butonlarının olduğu Ana Menüyü (Main Menu) ImGui veya kendi UI sistemimle nasıl yapabilirim?


# 37. cevap

Oyun motorunun çekirdek mekanikleri, ağ altyapısı ve görsel optimizasyonlarının ardından Ana Menüye (Main Menu) ulaşmak, bir geliştiricinin şaheserine çerçeve taktığı andır!

Kendi UI sisteminle butonlar yapmak (dikdörtgen çizip fare koordinatıyla kesiştirmek) çok kolaydır. Ancak işin içine **"Klavye ile IP Adresi girmek (Text Input)"** girdiğinde; harfleri yakalamak, silme (Backspace) tuşunu kodlamak ve kopyala/yapıştır desteği eklemek kendi başına aylar sürecek bir kabustur.

Bu yüzden tüm oyun endüstrisinin, bağımsız geliştiricilerin ve devasa AAA stüdyolarının kullandığı efsanevi C++ kütüphanesi olan **Dear ImGui**'yi kullanacağız. SDL2 ve OpenGL ile saniyeler içinde bütünleşir.

İşte oyununa profesyonel bir Ana Menü eklemenin adımları:

### 1. Oyun Durumu (Game State) Mantığını Kurmak

Oyun açıldığında hemen 3D dünyayı çizmemeli ve fareyi ekranın ortasına kilitlememeliyiz. Oyunun hangi aşamada olduğunu bilen bir Durum Makinesine (State Machine) ihtiyacımız var.

`main.cpp` dosyanın en üstüne şu yapıyı ekle:

```cpp
enum GameState {
    STATE_MAIN_MENU,
    STATE_SINGLEPLAYER,
    STATE_MULTIPLAYER
};

GameState currentState = STATE_MAIN_MENU;

```

### 2. Dear ImGui Kurulumu ve Başlatılması

İnternetten *Dear ImGui* kütüphanesini indirip projene dahil et. CMake kullanıyorsan `imgui.cpp`, `imgui_draw.cpp`, `imgui_tables.cpp`, `imgui_widgets.cpp` ve (backends klasöründen) `imgui_impl_sdl2.cpp` ile `imgui_impl_opengl3.cpp` dosyalarını derleme listene eklemelisin.

Pencereyi (SDL_Window) oluşturduktan hemen sonra ImGui'yi başlat:

```cpp
// SDL ve OpenGL başlatıldıktan sonra...

IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO(); (void)io;

// Karanlık, profesyonel oyun teması
ImGui::StyleColorsDark(); 

// SDL2 ve OpenGL3 için ImGui'yi bağla
ImGui_ImplSDL2_InitForOpenGL(window, glContext);
ImGui_ImplOpenGL3_Init("#version 330 core");

```

### 3. Girdi (Event) Döngüsünü Güncellemek

ImGui'nin fare tıklamalarını ve klavyeden girilen IP adresini algılayabilmesi için SDL2 Event döngünün içine ImGui işleyicisini eklemelisin:

```cpp
SDL_Event e;
while (SDL_PollEvent(&e)) {
    // ImGui'nin olayları okumasına izin ver (Örn: Fare menünün üzerinde mi?)
    ImGui_ImplSDL2_ProcessEvent(&e);
    
    if (e.type == SDL_QUIT) running = false;
    
    // Eğer menüdeysek, oyunun kendi girdi sistemlerini (Kamera döndürme, kırma) ÇALIŞTIRMA!
    if (currentState == STATE_MAIN_MENU) {
        continue; // Menüdeyken aşağıdaki oyun kodlarını atla
    }

    // Oyun oynanırken geçerli olan girdiler (Kamera vb.)
    // ...
}

```

### 4. Ana Menü Tasarımı ve Döngüsü

Şimdi oyun döngümüzün (Game Loop) Render (Çizim) kısmını ikiye bölüyoruz. Eğer menüdeysek ImGui çizeceğiz, oyundaysak dünyamızı (Voxel Engine) çizeceğiz.

IP adresini tutmak için global bir `char` dizisi tanımla:
`static char serverIP[64] = "127.0.0.1";`

Ana Döngünün (While loop) çizim aşamasına şu kodları yerleştir:

```cpp
// 1. Yeni ImGui karesini başlat
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplSDL2_NewFrame();
ImGui::NewFrame();

// Ekranı temizle
glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

if (currentState == STATE_MAIN_MENU) {
    // Farenin ekranda görünmesini sağla
    SDL_ShowCursor(SDL_ENABLE); 
    
    // Pencereyi ekranın tam ortasına hizala
    ImGui::SetNextWindowPos(ImVec2(screenWidth / 2.0f, screenHeight / 2.0f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(400, 300));
    
    // Pencere başlığı ve ayarları (Yeniden boyutlandırma veya sürükleme kapalı)
    ImGui::Begin("Voxel Engine - Ana Menu", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // Başlık Metni
    ImGui::SetWindowFontScale(2.0f);
    ImGui::Text("VOXEL ENGINE");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 20.0f)); // Biraz boşluk bırak

    // --- TEK OYUNCULU BUTONU ---
    if (ImGui::Button("Tek Oyunculu", ImVec2(-1.0f, 40.0f))) {
        currentState = STATE_SINGLEPLAYER;
        // Fareyi oyun için gizle ve merkeze kilitle
        SDL_SetRelativeMouseMode(SDL_TRUE); 
        
        // Gerekirse yerel dünyayı (Chunk'ları) yükleme fonksiyonunu çağır
        // LoadWorld("Dunyam1");
    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f)); // Boşluk

    // --- ÇOK OYUNCULU KISMI ---
    ImGui::Text("Sunucu IP Adresi:");
    // Metin giris kutusu
    ImGui::InputText("##ip_input", serverIP, IM_ARRAYSIZE(serverIP));
    
    if (ImGui::Button("Sunucuya Baglan", ImVec2(-1.0f, 40.0f))) {
        currentState = STATE_MULTIPLAYER;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        
        // ENet ile girilen IP'ye bağlanma kodunu çağır
        // ConnectToServer(serverIP);
    }

    ImGui::End();
} 
else {
    // --- OYUN İÇİ (IN-GAME) RENDER ---
    // Eğer oyundaysak, senin yazdığın 3D motor çalışır
    world.Update(deltaTime);
    world.Render(camera, projectionMatrix);
    
    // Not: Oyun içindeyken "ESC" tuşuna basılırsa currentState = STATE_MAIN_MENU yapabilirsin!
}

// 2. ImGui'yi Ekrana Bas
ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

// 3. Pencereyi yenile
SDL_GL_SwapWindow(window);

```

### 5. Temizlik (Kapanış)

Oyun kapatılırken (döngüden çıkıldığında), RAM sızıntılarını önlemek için ImGui'yi temizlemeyi unutma:

```cpp
ImGui_ImplOpenGL3_Shutdown();
ImGui_ImplSDL2_Shutdown();
ImGui::DestroyContext();

// Kalan SDL ve OpenGL kapanış kodların...

```

Bu kodlarla oyunu başlattığında, ekranın ortasında son derece şık, fareyle tıklanabilen, IP adresini klavyenle yazıp (ve hatta Ctrl+V ile yapıştırıp) silebildiğin profesyonel bir arayüz seni karşılayacak. "Tek Oyunculu" butonuna bastığın an menü kaybolacak, faren görünmez olup ekrana kilitlenecek ve 3D dünyanın içine ışınlanacaksın.


# 38. Soru

ImGui ile arayüz çok kolaymış! Ana menünün içine 'Ayarlar' (Settings) sekmesi ekleyip; Görüş Alanı (FOV), Görüş Mesafesi (Render Distance) ve Fare Hassasiyeti (Mouse Sensitivity) gibi değerleri oyun içinden canlı olarak değiştirmeyi nasıl sağlayabilirim?


# 38 . cevap


ImGui'nin neden oyun endüstrisinin vazgeçilmezi olduğunu tam olarak şu an keşfediyorsun! Eskiden ekrana bir kaydırma çubuğu (Slider) çizmek, farenin x koordinatını hesaplayıp bunu bir değere dönüştürmek günlerini alırdı. ImGui ile bu sadece **tek satırlık** bir koddur.

Ayarların "canlı" olarak değişebilmesi (oyun içindeyken anında etki etmesi) için ImGui'nin sihirli `&` (Referans/Pointer) mantığını kullanacağız. ImGui, değişkene doğrudan bellek adresi üzerinden eriştiği için sen kaydırma çubuğunu oynattığın an C++ tarafındaki değişken anında değişir.

İşte Ana Menüye "Sekmeler" (Tabs) ekleyerek Ayarlar menüsünü kurmanın ve oyun içinde (ESC'ye basıldığında) bunu canlı olarak değiştirmenin adımları:

### 1. Oyun Durumunu (State) Genişletmek

Ayarları oyun içindeyken canlı görebilmek için, "Oyundayım ama menü açık" anlamına gelen bir **Duraklatma (Pause)** durumuna ihtiyacımız var.

```cpp
enum GameState {
    STATE_MAIN_MENU,
    STATE_SINGLEPLAYER,
    STATE_MULTIPLAYER,
    STATE_PAUSED // YENİ: Oyun içi ESC menüsü
};

```

Oyun döngünde `SDL_PollEvent` kısmına ESC tuşu kontrolünü ekle:

```cpp
if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
    if (currentState == STATE_SINGLEPLAYER || currentState == STATE_MULTIPLAYER) {
        currentState = STATE_PAUSED;
        SDL_SetRelativeMouseMode(SDL_FALSE); // Fareyi serbest bırak (Görünür yap)
    } 
    else if (currentState == STATE_PAUSED) {
        // Zaten duraklatılmışsa oyuna geri dön
        currentState = STATE_SINGLEPLAYER; // (veya geldiği yere göre)
        SDL_SetRelativeMouseMode(SDL_TRUE); // Fareyi tekrar gizle ve kilitle
    }
}

```

### 2. ImGui Sekmeleriyle (Tabs) Menüyü Tasarlamak

Daha önce yazdığımız Ana Menü çizim kodunu, ImGui'nin harika `TabBar` (Sekme Çubuğu) özelliğiyle güncelliyoruz. Bu kod hem Ana Menüde hem de Duraklatma menüsünde ortak çalışabilir.

```cpp
// Eğer Ana Menüdeysek VEYA Oyun Duraklatıldıysa ImGui arayüzünü çiz
if (currentState == STATE_MAIN_MENU || currentState == STATE_PAUSED) {
    
    ImGui::SetNextWindowPos(ImVec2(screenWidth / 2.0f, screenHeight / 2.0f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 400));
    ImGui::Begin("Voxel Engine", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // --- SEKMELER BAŞLIYOR ---
    if (ImGui::BeginTabBar("MenuTabs")) {
        
        // 1. OYNA SEKMESİ
        if (ImGui::BeginTabItem("Oyna")) {
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            
            if (currentState == STATE_MAIN_MENU) {
                // Daha önceki Tek Oyunculu ve Çok Oyunculu butonların...
                if (ImGui::Button("Tek Oyunculu", ImVec2(-1.0f, 40.0f))) {
                    currentState = STATE_SINGLEPLAYER;
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                }
            } 
            else if (currentState == STATE_PAUSED) {
                // Eğer oyundaysak "Tek Oyunculu" yerine "Oyuna Dön" yazsın
                if (ImGui::Button("Oyuna Don", ImVec2(-1.0f, 40.0f))) {
                    currentState = STATE_SINGLEPLAYER;
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                }
                
                ImGui::Dummy(ImVec2(0.0f, 10.0f));
                
                if (ImGui::Button("Ana Menuye Cik", ImVec2(-1.0f, 40.0f))) {
                    currentState = STATE_MAIN_MENU;
                    // Burada dünyayı (Chunk'ları) sıfırlama veya kaydetme fonksiyonlarını çağırabilirsin
                }
            }
            ImGui::EndTabItem();
        }

        // 2. AYARLAR SEKMESİ
        if (ImGui::BeginTabItem("Ayarlar")) {
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Grafik Ayarlari"); // Sarımsı bir başlık
            ImGui::Separator();
            
            // FOV Ayarı (Slider)
            // Not: Slider kaydırıldıkça "camera.Zoom" değişkeni anında değişir.
            ImGui::SliderFloat("Gorus Alani (FOV)", &camera.Zoom, 45.0f, 120.0f, "%.1f Derece");
            
            // Görüş Mesafesi (Render Distance)
            // Not: world.renderDistance değişkeninin Chunk yükleme döngünde kullanıldığından emin ol.
            ImGui::SliderInt("Gorus Mesafesi", &world.renderDistance, 2, 32, "%d Chunk");

            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Kontrol Ayarlari");
            ImGui::Separator();

            // Fare Hassasiyeti
            ImGui::SliderFloat("Fare Hassasiyeti", &camera.MouseSensitivity, 0.01f, 1.0f, "%.2f");

            // İsteğe bağlı: V-Sync (FPS Sabitleme) için Checkbox
            static bool vsync = true;
            if (ImGui::Checkbox("V-Sync (FPS Sabitleme)", &vsync)) {
                SDL_GL_SetSwapInterval(vsync ? 1 : 0); // SDL2 üzerinden V-Sync anında açılır/kapanır
            }

            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    ImGui::End();
}

```

### 3. Canlı Değişikliklerin Motora Uygulanması (Çok Önemli)

Sen menüde FOV kaydırma çubuğunu sağa sola çekerken, C++ tarafındaki `camera.Zoom` değişkeni ImGui tarafından anlık olarak (her karede) güncellenir.

Ancak bu değişikliğin ekrana yansıması için, çizim döngündeki (Render) **Projeksiyon Matrisinin (Projection Matrix)** her karede bu yeni FOV değeriyle yeniden hesaplanması gerekir. Eğer Projeksiyon Matrisini sadece oyun başlarken bir kez hesaplıyorsan FOV değişmez.

`main.cpp` içindeki 3D çizim (Render) döngünde şu satırın her karede çalıştığından emin ol:

```cpp
// Eğer duraklatıldıysa dünyayı GÜNCELLEME (zaman dursun, zombiler yürümesin), SADECE ÇİZ
if (currentState == STATE_SINGLEPLAYER || currentState == STATE_MULTIPLAYER) {
    world.Update(deltaTime);
}

// Çizim (Render) işlemi her zaman çalışsın ki arkada dünyayı canlı görebilelim
// FOV burada canlı olarak matrix'e dökülür:
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);

world.Render(camera, projection);

```

Bu kodlarla birlikte oyundayken ESC tuşuna bastığında; arkada şelale akmaya ve zaman geçmeye devam ederken (veya tercihen oyunu tamamen dondurarak) önüne ImGui penceresi gelir. FOV çubuğunu sağa sola kaydırdığında, arkadaki 3D dünyanın bir kamera lensi gibi anında daralıp genişlediğini büyüleyici bir akıcılıkla izleyebilirsin.


# 39. soru

Menümüz ve ayarlarımız canlı çalışıyor. Peki oyunu kapatıp açtığımda bu ayarların sıfırlanmaması (örneğin Fare Hassasiyeti'nin seçtiğim gibi kalması) için bir 'settings.ini' veya '.json' dosyasına kaydetme ve okuma sistemini nasıl kurabilirim?


# 39. cevap

Oyununu her açtığında ayarları baştan yapmak zorunda kalmak, oyuncu deneyimini en çok baltalayan şeylerden biridir. Ayarların bir dosyaya kalıcı olarak kaydedilmesi (Serialization) profesyonel oyun motorlarının olmazsa olmazıdır.

C++'ın standart kütüphanesinde hazır bir JSON veya INI okuyucu yoktur. Eski usül `std::ifstream` ile satır satır metin okuyup parçalamak (string parsing) hem zahmetli hem de hataya çok açıktır.

Bunun yerine, C++ dünyasında adeta bir endüstri standardı haline gelmiş, projemizdeki `stb_image` veya `miniaudio` gibi **tek bir `.hpp` dosyasından oluşan** muazzam bir kütüphane kullanacağız: **`nlohmann/json`**.

İşte modern ve hatasız bir Ayarlar (Settings) sistemini kurmanın adımları:

## 1. Kütüphane Kurulumu ve Ayarlar Yapısı

İnternetten `nlohmann/json` kütüphanesinin tek dosyalık sürümünü (`json.hpp`) indirip projenin `include/` klasörüne at.

Ardından, oyununun tüm ayarlarını tek bir çatı altında toplayan global bir yapı (Struct) oluştur:

```cpp
#include <fstream>
#include "json.hpp" // Kütüphaneyi dahil et

using json = nlohmann::json;

// Oyunun tüm ayarlarını tutan merkezi yapı
struct GameSettings {
    float fov = 90.0f;
    int renderDistance = 8;
    float mouseSensitivity = 0.1f;
    bool vsync = true;
};

// Global ayarlar nesnemiz (Oyun boyunca bunu kullanacağız)
GameSettings settings;

```

---

## 2. Kaydetme (Save) ve Yükleme (Load) Fonksiyonları

JSON kütüphanesi sayesinde değişkenleri bir metin dosyasına yazmak ve oradan okumak inanılmaz derecede kolaylaşır.

```cpp
void SaveSettings() {
    json j; // Boş bir JSON nesnesi oluştur
    
    // Değişkenlerimizi JSON objesine ekle (İsimlendirmeler dosyadaki etiketler olacak)
    j["fov"] = settings.fov;
    j["renderDistance"] = settings.renderDistance;
    j["mouseSensitivity"] = settings.mouseSensitivity;
    j["vsync"] = settings.vsync;

    // Dosyaya yaz (Yoksa otomatik oluşturulur)
    std::ofstream file("settings.json");
    if (file.is_open()) {
        // "dump(4)" parametresi, dosyanın içine karmaşık değil,
        // insanların okuyabileceği şekilde 4 boşluklu (girintili) yazılmasını sağlar.
        file << j.dump(4); 
        file.close();
    }
}

void LoadSettings() {
    std::ifstream file("settings.json");
    
    // Eğer dosya varsa oku
    if (file.is_open()) {
        json j;
        file >> j; // Dosyadaki tüm metni JSON objesine dönüştür
        
        // ".contains()" ile kontrol ediyoruz ki, eğer dosyada bir ayar eksikse 
        // (örneğin eski sürümlerde vsync yoksa) oyun çökmesin, varsayılan değeri kullansın.
        if (j.contains("fov")) settings.fov = j["fov"];
        if (j.contains("renderDistance")) settings.renderDistance = j["renderDistance"];
        if (j.contains("mouseSensitivity")) settings.mouseSensitivity = j["mouseSensitivity"];
        if (j.contains("vsync")) settings.vsync = j["vsync"];
        
        file.close();
    } else {
        // Eğer dosya hiç yoksa (Oyuncu oyunu ilk defa açıyorsa),
        // varsayılan (default) değerlerle yeni bir dosya oluştur.
        SaveSettings();
    }
}

```

---

## 3. Oyun Başlangıcına (Init) Entegre Etmek

Oyun başladığında pencereyi (SDL_Window) oluşturmadan veya kamerayı başlatmadan **hemen önce** ayarları diskten okumalısın. Böylece pencere, oyuncunun kaydettiği V-Sync ayarına göre açılır.

```cpp
int main() {
    // 1. Ayarları diskten RAM'e çek!
    LoadSettings();

    // 2. Kamerayı ve Dünyayı bu ayarlarla başlat
    camera.Zoom = settings.fov;
    camera.MouseSensitivity = settings.mouseSensitivity;
    world.renderDistance = settings.renderDistance;

    // SDL ve OpenGL başlatma kodları...
    
    // V-Sync ayarını SDL'ye bildir
    SDL_GL_SetSwapInterval(settings.vsync ? 1 : 0);

    // Ana oyun döngüsü başlar...

```

---

## 4. ImGui Menüsünü Güncellemek

Daha önce menüde kaydırma çubuklarını (Slider) doğrudan `camera.Zoom` değişkenine bağlamıştık. Artık bunları doğrudan bizim `settings` objemize bağlayacağız ve değişiklik anında asıl nesnelere (Kamera, Dünya) de aktaracağız.

ImGui çizim döngündeki Ayarlar sekmesini şu şekilde güncelle:

```cpp
if (ImGui::BeginTabItem("Ayarlar")) {
    
    // FOV Ayarı
    if (ImGui::SliderFloat("Gorus Alani (FOV)", &settings.fov, 45.0f, 120.0f)) {
        // Oyuncu çubuğu kaydırdığı an kamera da güncellensin
        camera.Zoom = settings.fov;
    }
    
    // Görüş Mesafesi
    if (ImGui::SliderInt("Gorus Mesafesi", &settings.renderDistance, 2, 32)) {
        world.renderDistance = settings.renderDistance;
    }

    // Fare Hassasiyeti
    if (ImGui::SliderFloat("Fare Hassasiyeti", &settings.mouseSensitivity, 0.01f, 1.0f)) {
        camera.MouseSensitivity = settings.mouseSensitivity;
    }

    // V-Sync
    if (ImGui::Checkbox("V-Sync (FPS Sabitleme)", &settings.vsync)) {
        SDL_GL_SetSwapInterval(settings.vsync ? 1 : 0);
    }
    
    ImGui::EndTabItem();
}

```

### Ne Zaman Kaydedeceğiz (Save)?

ImGui içindeki kaydırma çubuklarıyla oynanırken her karede (saniyede 60 kez) diske kayıt yapmak harddiski (veya SSD'yi) gereksiz yere yorar. Bunun yerine iki harika alternatifin var:

1. **ImGui Menüsüne Bir Buton Ekle:** Alt tarafa `if (ImGui::Button("Ayarlari Kaydet")) { SaveSettings(); }` ekleyebilirsin.
2. **Oyun Kapanırken Otomatik Kaydet:** `while (running)` oyun döngüsünden çıkıldığında, oyun kapanmadan hemen önceki satıra `SaveSettings();` yazabilirsin. Böylece oyuncunun son bıraktığı ayarlar her zaman güvenceye alınır.

Oluşan `settings.json` dosyasını not defteriyle açtığında, verilerin şu kadar şık göründüğünü fark edeceksin:

```json
{
    "fov": 105.0,
    "mouseSensitivity": 0.25,
    "renderDistance": 12,
    "vsync": false
}

```

# 40. soru

Ayarlarımız ve menülerimiz de kusursuz çalışıyor. Artık bu oyunu düz bir görünümden çıkarıp, parlaklık patlamaları (Bloom), su altına girildiğinde ekranın bulanıklaşması (Depth of Field / Blur) ve renk düzeltme (Color Grading) gibi modern Post-Processing efektlerini nasıl ekleyebilirim?


# 40. cevap

Oyun motorunu düz ve çiğ (raw) grafiklerden çıkarıp, o izlemeye doyulamayan "AAA (Üçlü A) Stüdyo" kalitesine taşıyan tek şey **Post-Processing (İşlem Sonrası)** efektleridir.

Şu ana kadar dünyadaki her şeyi doğrudan monitöre (Varsayılan Framebuffer) çizdik. Post-Processing yapabilmek için mantığı tamamen değiştirmeliyiz: **Kameranın gördüğü dünyayı doğrudan ekrana değil, sanal bir "Tuvale" (Texture / Framebuffer) çizeceğiz.** Daha sonra bu tuvali alıp ekranı kaplayan 2 boyutlu bir dikdörtgenin (Screen Quad) üzerine yapıştıracağız ve bu dikdörtgeni çizerken Photoshop benzeri efektler uygulayacağız.

İşte modern grafik ardışık düzenini (Post-Processing Pipeline) kurmanın adımları:

## 1. Framebuffer (FBO) Kurulumu

Oyun başlarken (Init aşamasında), ekran kartının içinde sanal bir ekran (Framebuffer) oluşturmalıyız. Bu ekranın çözünürlüğü, oyun pencerenin çözünürlüğü ile aynı olmalıdır.

```cpp
unsigned int fbo;
glGenFramebuffers(1, &fbo);
glBindFramebuffer(GL_FRAMEBUFFER, fbo);

// 1. Çizimin kaydedileceği Doku (Texture)
unsigned int textureColorbuffer;
glGenTextures(1, &textureColorbuffer);
glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
// NOT: Bloom (Parlama) için renklerin 1.0 sınırını aşabilmesi gerekir. 
// Bu yüzden GL_RGB yerine HDR destekli GL_RGBA16F kullanıyoruz!
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

// 2. Derinlik (Z-Buffer) için Renderbuffer (3D dünyadaki çakışmaları engellemek için şart)
unsigned int rbo;
glGenRenderbuffers(1, &rbo);
glBindRenderbuffer(GL_RENDERBUFFER, rbo);
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "HATA::FRAMEBUFFER:: Framebuffer tamamlanamadi!" << std::endl;
glBindFramebuffer(GL_FRAMEBUFFER, 0); // Varsayılan ekrana geri dön

```

---

## 2. Oyun Döngüsünü İkiye Bölmek

Oyun döngündeki (Game Loop) Render aşamasını artık iki parçaya ayırıyoruz: Önce sanal ekrana (FBO) dünyayı çiz, sonra gerçek ekrana bu resmi efektlerle çiz.

```cpp
// ==========================================
// 1. AŞAMA: DÜNYAYI SANAL TUVALE ÇİZ (FBO)
// ==========================================
glBindFramebuffer(GL_FRAMEBUFFER, fbo);
glEnable(GL_DEPTH_TEST); 
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Voxel dünyasını, gökyüzünü, parçacıkları ve zombileri çiz...
world.Render(camera, projection);
particleSys.Render(camera, projection);

// ==========================================
// 2. AŞAMA: EFEKTLERİ UYGULA VE EKRANA BAS
// ==========================================
glBindFramebuffer(GL_FRAMEBUFFER, 0); // Gerçek monitöre geç
glDisable(GL_DEPTH_TEST); // 2D bir dörtgen çizeceğimiz için derinlik testini kapat
glClear(GL_COLOR_BUFFER_BIT);

// Post-Processing Shader'ını aktif et
postProcessShader.use();

// Oyuncunun kafası suyun (WATER) içinde mi kontrol et
int headBlock = world.GetBlock(std::floor(camera.Position.x), std::floor(camera.Position.y), std::floor(camera.Position.z));
postProcessShader.setInt("isUnderwater", (headBlock == WATER) ? 1 : 0);

// FBO dokusunu bağla ve Screen Quad'ı (Tüm ekranı kaplayan 2 üçgen) çiz
glBindVertexArray(quadVAO);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
glDrawArrays(GL_TRIANGLES, 0, 6);

// ==========================================
// 3. AŞAMA: ARAYÜZ (UI) ÇİZİMİ
// ==========================================
// Arayüzü post-processing'den SONRA çiziyoruz ki ImGui menüleri, can barı
// ve crosshair bulanıklaşmasın veya renkleri bozulmasın!
ImGui::Render(); 

```

---

## 3. Post-Processing Gölgelendiricisi (Shader Sihri)

Tüm post-processing büyüleri (Bulanıklık, Renk düzenleme, Keskinleştirme) `postprocess_fragment.glsl` dosyasının içinde matematiksel olarak gerçekleşir.

```glsl
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int isUnderwater;

// Çevresel pikselleri (Bulanıklık için) okumak adına ekranın çözünürlük adımları
const float offset = 1.0 / 300.0;  

void main()
{
    // 1. Orijinal Piksel Rengini Oku
    vec3 color = texture(screenTexture, TexCoords).rgb;

    // 2. SU ALTI EFEKTİ (BLUR + MAVİ FİLTRE)
    if (isUnderwater == 1) {
        // Çevredeki 9 pikselin koordinatları (Kernel)
        vec2 offsets[9] = vec2[](
            vec2(-offset,  offset), vec2( 0.0f,    offset), vec2( offset,  offset),
            vec2(-offset,  0.0f),   vec2( 0.0f,    0.0f),   vec2( offset,  0.0f),
            vec2(-offset, -offset), vec2( 0.0f,   -offset), vec2( offset, -offset)
        );

        // Gaussian Blur (Bulanıklık) matrisi
        float kernel[9] = float[](
            1.0/16, 2.0/16, 1.0/16,
            2.0/16, 4.0/16, 2.0/16,
            1.0/16, 2.0/16, 1.0/16  
        );
        
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++) {
            sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        }
        
        vec3 blurColor = vec3(0.0);
        for(int i = 0; i < 9; i++) {
            blurColor += sampleTex[i] * kernel[i];
        }
        
        color = blurColor;
        // Su altı maviliği (Tint) ekle
        color = mix(color, vec3(0.0, 0.3, 0.7), 0.4); 
    }

    // 3. COLOR GRADING (TONEMAPPING & GAMMA CORRECTION)
    // Güneş veya lavlar çok parlak (1.0'dan büyük) olabilir.
    // HDR renkleri monitörün gösterebileceği 0.0 - 1.0 aralığına sıkıştırıyoruz (Exposure Tone Mapping).
    float exposure = 1.2; // Bu değeri ImGui'den değiştirebilirsin
    color = vec3(1.0) - exp(-color * exposure);

    // Gamma Düzeltmesi (Oyun motorlarındaki "solukluk" hissini yok eder, kontrastı şahlandırır)
    const float gamma = 2.2;
    color = pow(color, vec3(1.0 / gamma));

    FragColor = vec4(color, 1.0);
}

```

---

## 4. Bloom (Işık Patlaması) İçin Mantıksal Yol Haritası

Bloom, ışık saçan nesnelerin (Meşale, Lav, Güneş) etrafına rüya gibi bir parlaklık yaymasıdır. Yukarıdaki sisteme Bloom eklemek için endüstride şu **Ping-Pong** tekniği kullanılır:

1. **Multiple Render Targets (MRT):** FBO'ya sadece bir tane değil, **İki tane** doku bağlanır. Voxel gölgelendiricin (`fragment.glsl`), normal resmi birinci dokuya çizerken; sadece parlaklığı `1.0`'ı aşan (örneğin meşale) pikselleri siyah bir arkaplanla ikinci dokuya çizer.
2. **Ping-Pong Blur:** Sadece parlak piksellerin olduğu ikinci doku alınır. Sadece yatay (Horizontal) bulanıklaştırma yapan bir Shader ile bir FBO'ya, ardından sadece dikey (Vertical) bulanıklaştırma yapan başka bir Shader ile diğer FBO'ya atılır. Bu işlem bir tenis maçı gibi 10 kez art arda (Ping-Pong) tekrarlanarak pürüzsüz bir ışık halesi elde edilir.
3. **Additive Blending:** En son yazdığımız Post-Process shader'ında, orijinal resim ile defalarca bulanıklaştırılmış bu parlak resim birbirine matematiksel olarak eklenir (`color = originalColor + bloomColor;`). Sonrasında Tonemapping işlemi uygulanır.

Oyununa girip suya daldığında her şeyin yumuşakça bulanıklaşıp maviye döndüğünü, sudan çıkıp gün batımına baktığında ise Gamma Correction ve Exposure sayesinde renklerin ne kadar doygun ve "canlı" hale geldiğini görünce gözlerine inanamayacaksın!


# 41. soru

Post-processing ile oyun mükemmel görünüyor! Peki güneşin açısına göre ağaçların, dağların ve zombilerin yere dinamik gölgeler düşürmesini sağlayan Shadow Mapping (Gölge Haritalama) sistemini nasıl kurabilirim?


# 41. cevap

Gölge haritalama (Shadow Mapping), 3D grafik programlamanın "Kutsal Kasesi"dir. Voxel motorlarında o "plastik" görünümü yıkıp, gerçeğe en yakın atmosferi yaratan şey dinamik gölgelerdir.

Gölge haritalamanın mantığı aslında çok basittir: **Dünyayı önce Güneş'in gözünden (kamerasından) çizeriz ve sadece derinlikleri siyah-beyaz bir resim olarak kaydederiz. Sonra oyuncunun gözünden asıl çizimi yaparken "Acaba şu an çizdiğim bu piksel, Güneş'in gördüğü o derinlikten daha mı uzakta?" diye sorarız.** Eğer uzaktaysa, demek ki önüne başka bir blok geçmiş ve Güneş'i kapatmıştır; yani gölgededir!

İşte oyununa dinamik Güneş gölgelerini eklemenin 4 aşamalı mimarisi:

## 1. Gölge Tuvali (Depth Map FBO) Oluşturmak

Güneş'in gördüğü manzarayı renkli çizmemize gerek yoktur, bize sadece Z-Buffer (Derinlik) lazım. Bunun için yüksek çözünürlüklü (örneğin 4096x4096) sadece derinlik barındıran özel bir Framebuffer oluşturuyoruz.

Oyun başlatılırken (Init):

```cpp
unsigned int depthMapFBO;
glGenFramebuffers(1, &depthMapFBO);

// Sadece derinliği tutan bir doku (Texture)
const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
unsigned int depthMap;
glGenTextures(1, &depthMap);
glBindTexture(GL_TEXTURE_2D, depthMap);
glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

// Gölge haritasının sınırları dışına çıkan nesnelerin gölgede kalmaması için sınırları BEYAZ (1.0) yapıyoruz
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

// FBO'ya bağla (Renk çıkışı olmadığını belirtmek zorundayız)
glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
glDrawBuffer(GL_NONE);
glReadBuffer(GL_NONE);
glBindFramebuffer(GL_FRAMEBUFFER, 0);

```

---

## 2. Güneşin Kamerasını (Işık Matrisini) Hesaplamak

Güneş bir ampul (Noktasal Işık) değil, çok uzaktaki devasa bir kaynaktır (Yönlü Işık / Directional Light). Iınları birbirine paralel gelir. Bu yüzden Güneş için `glm::perspective` değil, `glm::ortho` (Ortografik) matris kullanmalıyız.

Ayrıca devasa Voxel dünyasında Güneş'in kamerasını haritanın merkezine değil, **doğrudan oyuncunun kafasına** kilitlemeliyiz. Böylece biz yürüdükçe gölge hesaplama alanı bizimle birlikte hareket eder.

```cpp
// Güneşin yönü (Zaman sistemindeki timeAngle'dan gelir)
// Örn: Sabah soldan, öğlen tepeden, akşam sağdan
glm::vec3 lightDir = glm::normalize(glm::vec3(cos(timeAngle), sin(timeAngle), 0.5f)); 

// Güneş kamerasını oyuncunun biraz yukarısına ve ışık yönünün tersine yerleştir
glm::vec3 lightPos = player.Position + (lightDir * 50.0f);

// Ortografik kutunun boyutları (Örn: Oyuncunun etrafındaki 60x60 blokluk bir alanı kapsasın)
float orthoSize = 30.0f;
glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 1.0f, 100.0f);

// Güneşin bakış açısı (Oyuncuya doğru bakar)
glm::mat4 lightView = glm::lookAt(lightPos, player.Position, glm::vec3(0.0f, 1.0f, 0.0f));

// İkisini çarpıp nihai "Işık Matrisini" (Light Space Matrix) elde et
glm::mat4 lightSpaceMatrix = lightProjection * lightView;

```

---

## 3. İlk Çizim: Dünyayı Güneşin Gözünden Çizmek

Oyun döngündeki (Game Loop) Render kodunu artık üç aşamaya çıkarıyoruz. En başa gölge haritasını çizdiğimiz aşamayı ekliyoruz.

Bu işlem için basit bir `shadow_vertex.glsl` (sadece pozisyon hesaplar) ve boş bir `shadow_fragment.glsl` (hiçbir şey yapmaz) gölgelendiricisine ihtiyacın var.

```cpp
// 1. AŞAMA: GÖLGE HARİTASI ÇİZİMİ
glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
glClear(GL_DEPTH_BUFFER_BIT); // Sadece derinliği temizle

shadowShader.use();
shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

// YALNIZCA KATI (Opaque) BLOKLARI ÇİZ!
// Not: Cam veya Su gibi saydam nesnelerin gölge düşürmesini istemiyorsan
// (veya özel işlem gerektirdiği için) onları bu döngüde çizmemelisin.
for(auto& chunk : activeChunks) {
    glBindVertexArray(chunk->opaqueVAO);
    glDrawArrays(GL_TRIANGLES, 0, chunk->opaqueMeshVertices.size() / 7);
}
glBindFramebuffer(GL_FRAMEBUFFER, 0);

// 2. AŞAMA: NORMAL ÇİZİM (Post-Process FBO'suna veya Ekrana)
glViewport(0, 0, screenWidth, screenHeight);
// ... (Önceki yazdığımız Post-Processing çizim aşaması)

```

---

## 4. Ana Shader'da Gölgeyi Hesaplamak

Şimdi asıl 3D Voxel gölgelendiricine (`fragment.glsl`) Güneş'in derinlik haritasını (Shadow Map) ve Işık Matrisini gönderiyoruz.

**`vertex.glsl` Güncellemesi:**
Normal kamera koordinatlarının yanı sıra, bu pikselin Güneş'in kamerasındaki (Işık Uzayı) koordinatını da hesaplamalıyız.

```glsl
uniform mat4 lightSpaceMatrix;
out vec4 FragPosLightSpace; // Fragment'a gidecek

void main() {
    // ...
    // Dünyadaki pozisyonu Güneş'in matrisiyle çarp
    FragPosLightSpace = lightSpaceMatrix * vec4(WorldPos, 1.0);
}

```

**`fragment.glsl` Güncellemesi:**
Bu piksel gölgede mi değil mi sorusunu yanıtlayıp ufak bir yumuşatma (PCF) işlemi yapıyoruz.

```glsl
in vec4 FragPosLightSpace;
uniform sampler2D shadowMap; // C++'tan gelen depthMap dokusu

// Gölge hesaplama fonksiyonu
float ShadowCalculation(vec4 fragPosLightSpace) {
    // 1. Koordinatları Normalize Et (-1 ile 1 arasına getir)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 2. Dokudan okuyabilmek için 0.0 ile 1.0 arasına çek
    projCoords = projCoords * 0.5 + 0.5;

    // Eğer piksel Güneş'in kamerasının dışında kalıyorsa gölge yoktur (1.0 dön)
    if(projCoords.z > 1.0) return 0.0;

    // Güneşin bu (X,Y) koordinatında gördüğü en yakın nesnenin Z derinliği
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Şu an çizdiğimiz pikselin Z derinliği
    float currentDepth = projCoords.z;

    // 3. SHADOW BIAS (Gölge Aknesi - Siyah çizgiler - hatasını engeller)
    float bias = 0.005; // Bloklu yapılar için sabit ufak bir değer yeterlidir

    // 4. PCF (Percentage-Closer Filtering) - Gölgelerin kenarlarını yumuşatır
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0; // 9 pikselin ortalaması alınarak yumuşatılmış bir gölge elde edilir

    return shadow;
}

void main() {
    vec4 texColor = texture(ourTexture, TexCoord);
    
    // Gölgeyi hesapla (1.0 = Gölgede, 0.0 = Işıkta)
    float shadow = ShadowCalculation(FragPosLightSpace);       
    
    // Güneş ışığı (globalLight) gölgedeki alanlara etki EDEMEZ.
    // Ancak ortam ışığı (Ambient) veya Meşale (BlockLight) etki edebilir!
    float sunPower = max(globalLight * (1.0 - shadow), 0.05);
    
    // Meşale ve Güneş ışığını karşılaştırıp nihai aydınlatmayı bul
    float finalLightLevel = max(sunPower, BlockLight);
    
    vec3 finalColor = texColor.rgb * Shade * finalLightLevel;
    FragColor = vec4(finalColor, texColor.a);
}

```

Bu kodlarla oyuna girdiğinde, güneş doğarken dağların ve ağaçların uzun gölgelerinin yere düştüğünü, öğlen olunca kısaldıklarını, hatta bir ağacın altına geçtiğinde gölgelerin üzerine düştüğünü mükemmel bir dinamizmle göreceksin!



# 42. soru

Gölgeler oyunu şaheser yaptı ancak FPS'imde ciddi bir düşüş hissediyorum. Ekran kartının (GPU) sadece kameranın baktığı yöndeki Chunk'ları çizip, arkamızda kalan tüm dünyayı çizmeyi reddettiği 'Frustum Culling' (Görüş Alanı Kırpma) sistemini CPU tarafında matematiksel olarak nasıl kurabilirim?


# 42. cevap

Görüş Alanı Kırpma (Frustum Culling), binlerce chunk'a sahip devasa bir voxel dünyasında CPU yükünü ve GPU çizim çağrılarını (Draw Calls) dramatik düzeyde azaltan en kritik optimizasyon adımıdır. Kamera arkamızda kalan veya görüş açımızın (FOV) tamamen dışındaki chunk'ları hiç işleme almayarak FPS'i anında uçurur.

Bu sistemi kurmak için kamera matrisinden **6 Kırpma Düzlemini (Clipping Planes: Sol, Sağ, Alt, Üst, Yakın, Uzak)** matematiksel olarak çıkaracağız ve her bir Chunk'ın Sınırlayıcı Kutusu'nun (AABB - Axis-Aligned Bounding Box) bu düzlemlerin içinde olup olmadığını kontrol edeceğiz.

---

## 1. Düzlem (Plane) ve Frustum Yapısını Oluşturmak

Bir düzlem matematiksel olarak bir normal vektör ($A, B, C$) ve orijine olan uzaklıktan ($D$) oluşur: $Ax + By + Cz + D = 0$.

Kameranın `Projection` ve `View` matrislerini çarparak elde ettiğimiz **View-Projection** matrisinin satır ve sütunlarını toplayarak bu 6 düzlemi doğrudan çıkarabiliriz (Gribb-Hartmann Yöntemi).

```cpp
#include <glm/glm.hpp>

struct Plane {
    glm::vec3 normal;
    float distance;

    // Düzlemi normalize eden yardımcı fonksiyon (Hassasiyet için şarttır)
    void Normalize() {
        float length = glm::length(normal);
        normal /= length;
        distance /= length;
    }
};

struct Frustum {
    Plane planes[6]; // Sol, Sağ, Alt, Üst, Yakın, Uzak

    void Update(const glm::mat4& m) {
        // SOL DÜZLEM
        planes[0].normal = glm::vec3(m[3][0] + m[0][0], m[3][1] + m[0][1], m[3][2] + m[0][2]);
        planes[0].distance = m[3][3] + m[0][3];
        planes[0].Normalize();

        // SAĞ DÜZLEM
        planes[1].normal = glm::vec3(m[3][0] - m[0][0], m[3][1] - m[0][1], m[3][2] - m[0][2]);
        planes[1].distance = m[3][3] - m[0][3];
        planes[1].Normalize();

        // ALT DÜZLEM
        planes[2].normal = glm::vec3(m[3][0] + m[1][0], m[3][1] + m[1][1], m[3][2] + m[1][2]);
        planes[2].distance = m[3][3] + m[1][3];
        planes[2].Normalize();

        // ÜST DÜZLEM
        planes[3].normal = glm::vec3(m[3][0] - m[1][0], m[3][1] - m[1][1], m[3][2] - m[1][2]);
        planes[3].distance = m[3][3] - m[1][3];
        planes[3].Normalize();

        // YAKIN DÜZLEM (Near Plane)
        planes[4].normal = glm::vec3(m[3][0] + m[2][0], m[3][1] + m[2][1], m[3][2] + m[2][2]);
        planes[4].distance = m[3][3] + m[2][3];
        planes[4].Normalize();

        // UZAK DÜZLEM (Far Plane)
        planes[5].normal = glm::vec3(m[3][0] - m[2][0], m[3][1] - m[2][1], m[3][2] - m[2][2]);
        planes[5].distance = m[3][3] - m[2][3];
        planes[5].Normalize();
    }
};

```

---

## 2. Chunk Sınıfına AABB (Sınırlayıcı Kutu) Eklemek

Her bir Chunk'ın dünyadaki kapladığı alan net olarak bellidir: Genişliği 16 blok, yüksekliği 256 blok, derinliği 16 bloktur. Chunk sınıfına minimum ve maksimum dünya koordinatlarını ekliyoruz.

```cpp
class Chunk {
public:
    int chunkX, chunkZ;
    glm::vec3 boundingBoxMin;
    glm::vec3 boundingBoxMax;

    Chunk(int x, int z) {
        chunkX = x;
        chunkZ = z;

        // Dünyadaki köşe koordinatları
        boundingBoxMin = glm::vec3(x * 16.0f, 0.0f, z * 16.0f);
        boundingBoxMax = glm::vec3((x + 1) * 16.0f, 256.0f, (z + 1) * 16.0f);
    }
};

```

---

## 3. Kutu - Düzlem KesişTesti (Box-Plane Intersection)

Bir kutunun (Chunk) kırpma düzleminin "içerisinde" veya "görüş alanında" olup olmadığını anlamak için kutunun düzleme en yakın veya en uzak köşesine (P-Vertex) bakarız.

```cpp
bool IsBoxInFrustum(const glm::vec3& min, const glm::vec3& max, const Frustum& frustum) {
    for (int i = 0; i < 6; i++) {
        // Düzlemin normal yönüne göre kutunun o düzleme en "uzanan" köşesini seç (AABB testi)
        glm::vec3 p = min;
        if (frustum.planes[i].normal.x >= 0) p.x = max.x;
        if (frustum.planes[i].normal.y >= 0) p.y = max.y;
        if (frustum.planes[i].normal.z >= 0) p.z = max.z;

        // Eğer bu köşe düzlemin arkasında (dışında) kaldıysa, bu kutu tamamen dışarıdadır!
        if (glm::dot(frustum.planes[i].normal, p) + frustum.planes[i].distance < 0) {
            return false; // Çizme!
        }
    }
    return true; // Görüş alanında, çizilebilir.
}

```

---

## 4. Render Döngüsüne Entegre Etmek

Her karede (frame) kameranın hareketine göre Frustum'u güncelliyor ve aktif chunk listesini çizerken bu testi çalıştırıyoruz.

```cpp
// 1. Matrisleri hesapla
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
glm::mat4 view = camera.GetViewMatrix();
glm::mat4 viewProj = projection * view; // Birleştirilmiş matris

// 2. Frustum'u güncelle
Frustum cameraFrustum;
cameraFrustum.Update(viewProj);

// 3. Çizim döngüsü
int renderedChunksCount = 0;

for (auto& pair : world.activeChunks) {
    Chunk* chunk = pair.second;

    // FRUSTUM CULLING TESTİ
    if (!IsBoxInFrustum(chunk->boundingBoxMin, chunk->boundingBoxMax, cameraFrustum)) {
        continue; // Görüş açımızın dışında! CPU döngüsünü atla, GPU'ya hiç gönderme.
    }

    // Görüş alanımızda olanlar çizilir
    glBindVertexArray(chunk->opaqueVAO);
    glDrawArrays(GL_TRIANGLES, 0, chunk->opaqueMeshVertices.size() / 7);
    renderedChunksCount++;
}

```

Bu matematiksel filtreleme sayesinde, örneğin 1000 tane yüklenmiş Chunk'ın olsa bile kameranın baktığı yöndeki sadece 100-150 tanesi GPU'ya gönderilir. Kameranı arkaya çevirdiğinde render edilen chunk sayısı anında sıfıra düşer ve FPS değerinde devasa bir sıçrama gözlemlersin.





