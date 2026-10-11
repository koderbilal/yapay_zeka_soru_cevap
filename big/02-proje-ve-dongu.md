# Proje yapısı ve oyun döngüsü

## Klasör

Erken cevapta üç kaynak dosya vardı. 42 turun sığdığı ağaç bu. `main.cpp` oyun değildir.

```text
BenimMotorum/
├── CMakeLists.txt
├── include/                 üçüncü parti tek başlıklar
│   ├── stb_image.h
│   ├── json.hpp
│   ├── FastNoiseLite.h
│   └── miniaudio.h
├── assets/                  Bedrock pack buraya ayrıştırılır
│   ├── textures/atlas.png
│   ├── textures/ui/         hotbar, kalp, artı, joystick
│   ├── fonts/ascii.png
│   ├── sounds/
│   └── shaders/
├── saves/                   çalışma anında oluşur, repoya girmez
│   └── Dunyam1/
│       ├── player.dat
│       └── chunks/{x}_{z}.chunk
├── settings.json
└── src/
    ├── main.cpp             pencere, döngü, başka hiçbir kural yok
    ├── core/                zaman, girdi, ReadFile, GameSettings
    ├── world/               Registry, Chunk, World, Generator, Light, Save
    ├── player/              Camera, Player, Physics, Inventory, Crafting, Mining
    ├── entity/              Entity, Chicken, Zombie, DroppedItem
    ├── render/              Shader, Atlas, ChunkRenderer, Frustum,
    │                        Model, Particle, Weather, Sky, HUD, Post
    ├── audio/               AudioEngine
    ├── ui/                  ImGui ana menü ve ayar
    └── net/                 boş. Yol haritasının son mili
```

Her sınıfın `.h` ve `.cpp`'si aynı adlı kalsın. Başlıkta OpenGL çağrısı olmasın.

## CMake'in omurgası

İsim: `VoxelEngine`. C++17. `src/` altındaki bütün `.cpp` tek `add_executable`. Bağlananlar: `OpenGL::GL`, `SDL2::SDL2`. GLM ve tek başlıklar include.

Masaüstü kurulumu (Debian):

```bash
sudo apt install build-essential cmake libsdl2-dev libglm-dev
cmake -S . -B build && cmake --build build
./build/VoxelEngine
```

Android dalı aynı `CMakeLists.txt` içinde `if(ANDROID)` ile GLES ve NDK'ya döner. Ayrı proje açılmaz. Ayrıntı `09-android.md`.

ImGui, ENet ve miniaudio kaynakları bu listeye **o mil taşında** eklenir. İlk derlemede hepsi durmasın.

## Kimin neye sahip olduğu

Tek yazma kapısı kuralı. İki sistem aynı diziyi keyfi değiştirirse kayıt, ağ ve mesh birbirini ezer.

| Veri | Sahip | Başkası ne yapabilir |
|---|---|---|
| `blocks[]`, `lightMap[]` | `Chunk`, ama yazan yalnız `World::SetBlock` | Okur. Mesh kirli işareti World koyar |
| Yüklenen chunk haritası | `World` | Renderer yalnız görünür olanı çizer |
| Blok kuralları | `Registry` (`BlockDef`) | Herkes okur, kimse çalışma anında değiştirmez |
| Bakış | `Camera` | Player fizik sonucu konumu yazar |
| Can, açlık yeri, envanter, el | `Player` | UI okur |
| Canlılar, düşen eşya | `Entity` listesi, sahibi `World` veya `Game` | Render okur |
| GPU mesh (VAO/VBO) | `Chunk`, yükleyen ana thread | Worker yalnız `vector<float>` doldurur |
| Ayar | `GameSettings` | Menü yazar, kamera ve frustum okur |
| Ses | `AudioEngine` | Oyun olayları `Play` der |

`SetBlock(x,y,z,id)` şunları tek yerde yapar: diziyi değiştir, ışığı yay veya söndür, o chunk'ı ve gerekirse komşuyu kirli işaretle, düşen eşyaysa onu ayrıca çağıran kırma kodu üretir, ağ açıksa paketi World değil Net katmanı yollar.

## Oyun durumları

`Game` bir enum tutar: `MainMenu`, `Playing`, `Paused`.

- Menü: dünya tick'lenmez. ImGui. Tek oyuncu dünyayı yükler, çok oyunculu IP sorar (ağ milinden önce buton durabilir, işlevsiz kalmasın ya da gizlensin).
- Oyun: aşağıdaki kare.
- Duraklatma: simülasyon donar, menü üstte, fare serbest.

## Bir kare

`dt` saniye. Üst sınır koy (örneğin 0.05), debugger'da zıplama dünyayı delmesin.

1. Girdi. Masaüstü: SDL klavye + yakalanmış fare. Android: parmak kimliği, sanal joystick. ImGui açıksa fare oyuna gitmez.
2. Ağ varsa al/gönder (`10-ag.md`). Tek oyuncuda bu adım yok.
3. `World::Update(playerPos)` — uzaktaki chunk'ı kaydet ve at, yakını kuyruğa al, bitmiş mesh'i GPU'ya ver.
4. Fizik. Creative uçuş kapalıysa yerçekimi, zıplama, eksen eksen AABB.
5. Madencilik sayacı (sol tık basılı). Dolunca kırma.
6. Sağ tık: elde bloksa koy, çalışma masasıysa menü, yemeksse can (yemek henüz tam tarif değil, kanca durur).
7. Canlılar ve düşen eşyalar.
8. Parçacık, yağmur/kar.
9. Dinleyici = kamera. Yürüyorsa ayak sesi sayacı.
10. Çizim, bu sırayla:
    - gölge haritası (yoksa atla)
    - gökyüzü rengi temizliği
    - frustum içi opak chunk
    - varlık modelleri
    - saydam chunk, kameradan uzağa göre
    - parçacık ve hava
    - güneş / ay
    - post (yoksa atla)
    - HUD (derinlik kapalı)
    - ImGui (menü veya duraklatma)
11. `SDL_GL_SwapWindow`.
12. Çıkışta oyuncu + kirli chunk'lar diske.

## main.cpp'nin tamamı bu kadar

```cpp
int main(int argc, char** argv) {
    App app;
    if (!app.Init(argc, argv)) return 1;
    app.Run();   // yukarıdaki kare, durum makinesi içinde
    app.Shutdown();
    return 0;
}
```

`App` SDL penceresi, GL bağlamı, `GameSettings` yükleme, `Registry` yükleme, `Game`. Oyun kuralı burada birikmez.
