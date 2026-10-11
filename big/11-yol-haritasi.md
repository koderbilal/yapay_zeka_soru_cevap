# Yol haritası

Sohbetin sırası bu liste değildir. Üstteki mil bitmeden alttakine kod yazılmaz. "Bitmek": o maddenin sahnesi ekranda görünür, kapanınca dünya durur, derleme temizdir.

## Mil 0 — pencere

SDL2 penceresi, GL 3.3, bir küp, Bedrock dirt, `GL_NEAREST`, uçan kamera, WASD + fare. `ReadFile` masaüstünde çalışır. CMake `build/` verir.

Dosyalar: `main.cpp`, `core/File`, `render/Shader`, `player/Camera`.

## Mil 1 — yer

Chunk, yüz eleme, atlas, bir düzlem arazi (gürültüsüz de olur). Chunk başına bir draw. Havada duran oyuncu.

`world/Chunk`, `world/Registry` (en azından air, dirt, stone), `render/ChunkRenderer`.

## Mil 2 — beden

Yerçekimi, zıplama, eksen çarpışması, creative uçuş tuşu. Işınla anında kırma ve koyma (süre ve düşen eşya sonra). Artı.

Bu milde oyun "Minecraft'a benzeyen bir yürüyüş"tür. Menü yok.

## Mil 3 — eşya

Hotbar, 36 yuva, ikon, adet yazısı, kalp (sabit 20, hasar sonra). Kırma süresi ve alet çarpanı. Kırılınca düşen eşya, yaklaşınca yuva. Çalışma masası, tahta, çubuk, kazma, kılıç.

Anında envantere ekleyen eski kırma yolu bu milde yazılmaz bile.

## Mil 4 — dünya karakteri

Yükseklik + sıcaklık, orman / çöl, ağaç, su ikinci geçiş, mağara, demir ve elmas damarı. Yüz ışığı ve AO. Gündüz-gece çarpanı (meşalesiz de olur). Sonra meşale, yayma **ve** söndürme.

İşçi havuzu ve frustum bu milin sonunda, arazi kasınca. Daha önce erken optimizasyon.

## Mil 5 — hayat

Tavuk, zombi, hasar, iframes, ölünce düşen eşya, küp modeli, yürüme. Parçacık havuzu. miniaudio: adım, kırma, zombi. Yağmur / kar.

## Mil 6 — kapanıp açılma

Binary chunk + oyuncu. Esc menüsü, ayarlar JSON, FOV ve görüş mesafesi canlı. Dünya adı hâlâ `Dunyam1` olabilir.

## Mil 7 — pahalı görüntü

Ayar anahtarıyla bloom, su altı, gölge. Kapalı varsayılan. Frustum zaten vardır; gölge onu yeniden yazmaz.

## Mil 8 — telefon

GLES öneki, `SDL_RWFromFile`, kayıt yolu ayrı, joystick. Masaüstü kontrolü durur.

## Mil 9 — ağ

`update` / `render` ayrımı bu milin ilk işi, paket değil. Sonra ENet, RLE chunk, blok tahmini, hareket tahmini, uzaktaki oyuncunun mix'i. Sunucu ayrı çalıştırılır, penceresiz.

## Henüz tasarlanmayan, iskeleti açık olanlar

| İstek | Nereye takılır | Şimdi yapılmaz çünkü |
|---|---|---|
| Açlık | `Player.hunger`, ikinci HUD sırası, yemek sağ tık | mekanik hiç yazılmadı |
| Boss | `Entity` alt sınıfı, fazla can, evre | yalnızca ilk mesajda geçti |
| Mod | `assets/data/blocks.json` → `Registry` | API yok, tablonun şekli hazır |
| Canlı kaydı | `player.dat` yanına `entities.dat` | v1 yeniden doğuruyor |
| Greedy mesh | `Chunk::GenerateMesh` içi | yüz eleme yetene kadar yok |
| Gerçek su akışı | `SetBlock` sonrası başka bir BFS | şimdi durgun kaynak |
| A* zombi | `Zombie::Update` | bir blok zıplama yeter |

## Bir tur kod yazılırken

1. İlgili dosyayı aç (`03` dünya, `04` eşya, …). Başka turun yarım snippet'ini yapıştırma.
2. Yeni sınıf `02`'deki klasöre girer, `main.cpp`'ye birikmez.
3. Veriyi sahibi olmayan sistem yazamaz. Tablo `02`'de.
4. Aynı işi ikinci kez ekleme: frustum, kırma→envanter, kamera-içinde-can, blok başına PNG. Bunlar `01`'de kapatıldı.
