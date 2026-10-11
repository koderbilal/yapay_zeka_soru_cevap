# Vizyon ve sabitlenen kararlar

## Oyun

C++ ile yazılmış, kendine ait motoru olan bir sandbox.

İstenen çekirdek, ilk mesajdan:

- Kare kare (voxel) dünya. Blok ve eşya adları büyük ölçüde Minecraft adları, çünkü elde Bedrock doku paketi ve ona ait asset'ler var. Onlar kullanılacak, sıfırdan çizim yok.
- Envanter.
- Can barı ve açlık barı.
- Canlılar, yaratıklar, bosslar.
- İleride mod. Bu yüzden Godot değil, **özel motor**.

42 turun sonunda gerçekten tasarlananlar: dünya, oyuncu, envanter, üretim, tavuk, zombi, kayıt, menü, Android, ağ, post-process. **Açlık mekaniği, boss ve mod yükleyici tasarlanmadı.** İskelet onları sonradan takacak şekilde kurulur; şimdi uydurulmaz.

## Neden özel motor

Godot + C++ UI ve fiziği hazır verirdi, voxel çizimini yine sen yazardın. Seçilen yol sıfırdan motor:

- Chunk belleği ve blok güncellemesi sende.
- Modlama için başkasının sahne grafiğine kilitlenmemek.
- Aynı çekirdek masaüstü ve Android.

Bedeli: çarpışma, UI, ses, pencere ve derleme de sende. Hazır motorun işi kütüphanelere bırakılır, oyun kuralı bırakılmaz.

## Sabit yığın

| İş | Seçim | Neden bu, diğeri değil |
|---|---|---|
| Dil | C++17 | CMake cevabında kilitlendi |
| Derleme | CMake ≥ 3.15 | Windows, Linux, Android NDK aynı dosya |
| Pencere ve girdi | SDL2 | GLFW değil; mobil ve dosya okuma daha düz |
| Grafik | OpenGL 3.3 Core, Android'de OpenGL ES 3.0 | Vulkan yok |
| Matematik | GLM | |
| PNG | stb_image, bellekten (`stbi_load_from_memory`) | |
| Gürültü | FastNoiseLite | Perlin / biyom / mağara |
| Ayar ve blok tanımı | nlohmann/json (`json.hpp`) | INI elendi |
| Ses | miniaudio | OpenAL ve SDL_mixer elendi; bellekten çalar, tek başlık |
| Menü | ImGui | Yalnız menü ve ayar. Kalp ve hotbar ham quad |
| Ağ | ENet, port 7777 | TCP değil. İki kanal: 0 güvenilir, 1 hızlı |
| Dünya kaydı | Binary, chunk başına dosya | JSON dünya için yasak |

IDE: VS Code + CMake. Derleme `build/` içinde, kaynakla karışmaz.

## Birleştirirken değiştirilen çelişkiler

Sohbet her turda bir öncekinin üstüne kod yapıştırdı. Aşağıdakiler artık tek doğru.

1. **Kamera oyuncu değildir.** Erken `Camera` can ve envanter taşıyordu. `Camera` yalnız bakış: `Position`, `Front`, `Up`, `Yaw`, `Pitch`, `GetViewMatrix()`. Can, envanter, fizik `Player`da.
2. **Uçan kamera kalkmaz, varsayılan olmaz.** Fizik gelince yerçekimi esas. Uçuş creative anahtarı. İkisi birden `Position` yazmaz.
3. **Kırma envantere ışınlamaz.** 9. tur böyleydi, 28. tur düşen eşyayla değiştirdi. Geçerli olan: blok havaya döner, `DroppedItem` doğar, yaklaşınca yuvaya girer.
4. **Frustum bir kez yazılır.** 15 ve 42 aynı sistem. Tek `Frustum`, tek test, chunk çizmeden hemen önce.
5. **Doku tek PNG değil, atlas.** `GL_NEAREST` şart, yoksa 16×16 bulanıklaşır. Küp UV'si atlas hücresinden kesilir.
6. **Chunk dizisi `uint8_t`.** 0–255 yerleştirilebilir blok. 256 ve üstü yalnız envanter eşyası (elma, tüy, çiğ tavuk, çürük et, alet). Eşya kimliği chunk'a yazılmaz.
7. **İki mesh.** Opak ve saydam (su, yaprak, cam). Yaprak ve camda saydam piksel `discard`. Su ikinci geçişte, arkadan öne.
8. **Mesh üretimi işçi iş parçacığında, `glBufferData` ana iş parçacığında.** OpenGL çağrısı worker'da yok.
9. **Varlıklar sınıf, ECS değil.** İlk cevap ECS demişti; tavuk ve zombi sonra düz sınıf olarak yazıldı. v1: `Entity` taban, `Chicken` / `Zombie` / `DroppedItem` türetir. Ortak veri (konum, hız, can, AABB) tabanda.
10. **HUD ile menü ayrı çizilir.** Kalp, hotbar, sayı, artı, joystick: tek 1×1 quad + ortho. Ana menü ve ayar: ImGui.
11. **Dosya yolu doğrudan `ifstream` ile asset okumaz.** Tek `ReadFile(path) -> bytes`. Masaüstünde disk, Android'de `SDL_RWFromFile`. stb ve miniaudio bu baytı yer. Shader da aynı kapıdan string olur.
12. **Ayar JSON.** `settings.json`: FOV, görüş mesafesi, fare hassasiyeti, vsync.
13. **Sunucu çizmez.** Ağ açılınca simülasyon (`update`) çizimden (`render`) ayrı durmak zorunda. Sunucu dünya + fizik + canlı. İstemci çizer.

## Bilerek dışarıda bırakılanlar

- Boss. Yer tutucu: ileride kalın bir `Entity` (fazla can, evre). Şimdi sınıf açma.
- Açlık **barı** istendi, **mekanik** yazılmadı. Kalp çizimi açlık değildir. Sonra `Player.hunger`, HUD'da ikinci ikon sırası.
- Script'li mod yükleyici yok. Mod kapısı, başlangıçta JSON'dan dolan `BlockDef` tablosu. Yeni blok = yeni satır, yeni C++ sınıfı değil.
- Greedy meshing yok. v1 yüz eleme (face culling) yeter. Greedy sonra, FPS yetmezse.
- Vulkan, fizik motoru (Bullet/PhysX), hazır UI kiti yok.
