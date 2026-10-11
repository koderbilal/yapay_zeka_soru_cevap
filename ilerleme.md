# İlerleme

Bu dosya projenin nerede olduğunu ve sırada ne olduğunu özetler. Ayrıntılı tasarım için `big/` klasörüne, sıralama için `big/11-yol-haritasi.md` dosyasına bak.

**Şu an:** Mil 0 ve Mil 1 tamam, dünya üretimi ilk hâliyle eklendi. Sırada Mil 2 (beden) var.

## Proje

C++ ile Minecraft benzeri bir sandbox / voxel oyunu. Özel motor kullanıyoruz: SDL2 (pencere, girdi), OpenGL 3.3 (çizim), GLM (matematik), CMake (derleme). Dokular Bedrock texture pack'inden geliyor ve `Assets/` klasöründe duruyor. İleride Android ve çok oyunculu destek planlı.

## Yapılanlar

### Tasarım
- [x] 42 soru-cevaplık sohbet (`ben.md`) konuya göre toparlandı, `big/` klasörüne yazıldı.
- [x] Vizyon, klasör yapısı, dünya, oyuncu/eşya, canlılar, ekran/ses, performans, kayıt/menü, Android ve ağ kararları yazıldı.
- [x] Yol haritası çıkarıldı (`big/11-yol-haritasi.md`).

### Mil 0 — pencere (tamamlandı)
- [x] CMake projesi (`code/CMakeLists.txt`), `build/` çıktısı veriyor.
- [x] SDL2 penceresi ve OpenGL 3.3 core bağlamı.
- [x] Shader yükleme (`render/Shader`), dosya okuma (`core/File`).
- [x] Tek küp çiziliyor, Bedrock `dirt.png` dokusu `GL_NEAREST` ile kullanılıyor. PNG yoksa üretilmiş bir yedek doku devreye giriyor.
- [x] Uçan kamera (`player/Camera`): WASD, Space, Ctrl/Shift, fare ile bakma, Esc ile fareyi bırakma.
- [x] Linux'ta (CachyOS dahil) derlenip çalıştığı doğrulandı: pencerede dirt küpü görünüyor.

### Mil 1 — yer (tamamlandı)
- [x] `world/Registry`: blok kimlikleri, yüz başına doku, küp / çapraz şekil.
- [x] `world/Chunk` (16×256×16), `world/World`, yüz eleme, chunk başına tek draw (`render/ChunkRenderer`).
- [x] `render/TextureAtlas`: 19 doku tek atlasta, `GL_NEAREST`.
- [x] Dünya üretimi (`world/WorldGen`): yükseklik gürültüsü, çöl/ova, granit-diyorit-andezit, mağara, cevherler, bedrock, ağaç gövdesi, fidan. Tohum: `VoxelEngine [seed]`.
- [ ] Eksik: su, yaprak, çimen dokusu, ışık, dinamik chunk yükleme (Mil 4).

### Dokular
- [x] 19 blok dokusu `Assets/` içinde, dosya adı = blok adı: `bedrock`, `clay`, `coal_ore`, `cobblestone`, `dirt`, `gold_ore`, `gravel`, `iron_ore`, `lapis_ore`, `log_oak`, `log_oak_top`, `obsidian`, `planks_oak`, `sand`, `sapling_oak`, `stone`, `stone_andesite`, `stone_diorite`, `stone_granite`.
- [x] Hepsi `code/assets/textures/blocks/` altına kopyalandı ve oyunda kullanılıyor (doku atlası).

## Sıradaki adımlar

Kural: üstteki mil bitmeden alttakine kod yazılmaz. "Bitmek": ekranda görünür, kapanınca dünya durur, derleme temizdir.

| Mil | Konu | Özet |
|---|---|---|
| 2 | Beden | Yerçekimi, zıplama, çarpışma, creative uçuş, ışınla blok kırma/koyma, artı işareti. |
| 3 | Eşya | Hotbar, 36 yuvalı envanter, kalpler, kırma süresi, düşen eşya, çalışma masası ve ilk aletler. |
| 4 | Dünya karakteri | Yükseklik + sıcaklık, biyomlar, ağaç, su, mağara, cevher damarları, ışık, gündüz-gece, meşale. İşçi havuzu ve frustum eleme. |
| 5 | Hayat | Tavuk ve zombi, hasar, ölüm, parçacıklar, ses (miniaudio), yağmur/kar. |
| 6 | Kapanıp açılma | Binary kayıt, Esc menüsü, ayarlar (JSON), FOV ve görüş mesafesi. |
| 7 | Pahalı görüntü | Bloom, su altı efekti, gölge (ayarla açılıp kapanır). |
| 8 | Telefon | Android: GLES, APK içinden dosya okuma, joystick ve dokunmatik kontrol. |
| 9 | Ağ | `update` / `render` ayrımı, ENet, çok oyunculu, penceresiz sunucu. |

### Henüz tasarlanmamış (iskeleti açık)
Açlık, boss, mod desteği, canlıların kaydı, greedy mesh, gerçek su akışı, A* ile zombi yolu.

## Hemen yapılacaklar (Mil 1 için)
1. `world/Chunk` ve `world/Registry` sınıflarını ekle.
2. Blok dokularını tek bir atlasta birleştir.
3. Yüz eleme ile chunk mesh'i üret.
4. Düz bir arazi çiz ve kamerayı üstüne yerleştir.
