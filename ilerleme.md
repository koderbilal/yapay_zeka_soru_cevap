# İlerleme

Bu dosya projenin nerede olduğunu ve sırada ne olduğunu özetler. Ayrıntılı tasarım için `big/` klasörüne, sıralama için `big/11-yol-haritasi.md` dosyasına bak.

**Şu an:** Mil 0, 1 ve 2 tamam. Mil 3'ün envanter, hotbar, kırma süresi, düşen eşya, kalp ve çalışma masası kısmı da kodda. Sırada kalan Mil 3 artıkları (zırh giyme, fırın) ve Mil 4'ün ışık / gündüz-gece / chunk akışı kısmı.

## Proje

C++ ile Minecraft benzeri bir sandbox / voxel oyunu. Özel motor: SDL2 (pencere, girdi), OpenGL 3.3 (çizim), GLM (matematik), CMake (derleme). Dokular Bedrock pack'inden: `Assets/`, `Extra/`, `Items/`. İleride Android ve çok oyunculu destek planlı.

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
- [x] `render/TextureAtlas`: blok ve eşya atlası, `GL_NEAREST`.
- [x] Dünya üretimi (`world/WorldGen`): yükseklik, çöl/ova, katmanlar, mağara, cevher, bedrock. Tohum: `VoxelEngine [seed]`.

### Mil 2 — beden (tamamlandı)
- [x] `player/Player`: ayak konumu, 0.6×1.8 kutu, göz 1.62.
- [x] Yerçekimi, zıplama, eksen eksen çarpışma, basamak çıkma.
- [x] `F` ile creative uçuş (yerçekimi ve çarpışma yok).
- [x] Işınla kırma (süre + alet çarpanı) ve sağ tıkla koyma. Artı imleç.
- [x] Su yavaşlatır ve yüzdürür; lav ve kaktüs hasar verir. Ölünce doğuş noktasına dönüş.

### Mil 3 — eşya (büyük kısmı)
- [x] Hotbar (1–9 ve tekerlek), 36 yuva, `E` ile 2×2 envanter, çalışma masası sağ tık.
- [x] Sol tık: al, birleştir, değiştir. Sonuç yuvası tarifi uygular.
- [x] Tarifler: kütük→tahta, tahta→çubuk, 2×2 tahta→masa, tahta ve taş kazma/balta/kürek/çapa/kılıç.
- [x] Alet dayanıklılığı. Düşen küp eşya, yaklaşınca hotbar sonra çanta.
- [x] Kalpler kırmızı dörtgen olarak çizilir (kalp dokusu yok). Açlık alanı durur, mekanik yok.
- [x] Envanter açıkken boş zırh ve kalkan yuvaları `empty_armor_slot_*` dokusunu gösterir. Uygun parça tıklanınca yuvaya girer; koruma hesabı yok.
- [ ] Fırın, yemek sağ tık, zırhın hasar azaltması yok.

### Dünya (Mil 4'ten erken gelenler)
- [x] Çöl / ova, kırmızı kum, okyanus (deniz seviyesi 62), derin mağarada lav cebi.
- [x] Çölde kaktüs (1–3) ve ölü çalı. Meşe gövdesi + 5×5 yaprak tacı.
- [x] Su yalnızca açılan hücreden aşağı akar (yana taşmaz).
- [ ] Yüz ışığı, AO, gündüz-gece, meşale, dinamik chunk yükleme, frustum yok.

### Dokular
- [x] `Assets/` blokları ve `Extra/` (çimen, yaprak, kaktüs, su, lav, çalışma masası, çim yolu, kırmızı kum) `code/assets/textures/blocks/` altında.
- [x] `Items/` eşyaları `code/assets/textures/items/` altında, kimlikler 256'dan başlar.
- [x] Animasyon şeritleri (su, lav) ilk kareyi kullanır. Çimen ve yaprak yeşil tint alır.

## Sıradaki adımlar

| Mil | Konu | Özet |
|---|---|---|
| 3 kalan | Eşya | Zırh, fırın, yemek. Kalp/hotbar dokusu isteğe bağlı. |
| 4 | Dünya karakteri | Işık, gündüz-gece, meşale, işçi havuzu, frustum, gerçek su akışı. |
| 5 | Hayat | Tavuk ve zombi, hasar, parçacık, ses, yağmur/kar. |
| 6 | Kayıt | Binary dünya, Esc menüsü, ayarlar. |
| 7 | Görüntü | Bloom, su altı, gölge (kapalı varsayılan). |
| 8 | Telefon | GLES, dokunmatik. |
| 9 | Ağ | `update` / `render` ayrımı, ENet. |

### Henüz yok
Açlık mekaniği, boss, mod JSON, canlı kaydı, greedy mesh, yana su akışı, A* zombi yolu, kırılma parçacığı ve ses.
