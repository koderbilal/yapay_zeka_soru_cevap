# Kayıt, menü, ayar

Üç dosya türü, üç iş. Karıştırılmaz.

| Ne | Dosya | Biçim |
|---|---|---|
| Ayar | `settings.json` | JSON, küçük |
| Oyuncu | `saves/<ad>/player.dat` | binary |
| Arazi | `saves/<ad>/chunks/<x>_<z>.chunk` | binary |
| Blok tanımları (ileride) | `assets/data/blocks.json` | JSON |

Dünya JSON'u yasak. Bir chunk 65536 blok; metin hem şişer hem kareyi kilitler.

## Chunk dosyası

v1 düz döküm: `blocks` 65536 bayt, ardından `lightMap` 65536 bayt. Sürüm baytı başta: `uint32 version = 1`. Işık yazılmazsa yükleyince yeniden hesaplanır; ikisini de yazmak açılışı hızlandırır.

Dosya yoksa üretim. Varsa üretim yok, tohum yok sayılır.

Ne zaman yazılır:

- Chunk görüş mesafesinden çıkıp RAM'den silinmeden hemen önce, değiştiyse.
- Oyun kapanırken bütün kirli chunk'lar ve oyuncu.
- Her blokta yazılmaz.

İleride RLE (ağ bölümündeki aynı fikir) dosyayı küçültür. Sürüm baytı bunun için başta durur. v1'i RLE yapma; önce düz döküm çalışsın.

## Oyuncu dosyası

Sıra sabit, struct hizasına güvenilmez; alan alan yaz:

1. position, 3 float
2. yaw, pitch
3. health
4. activeSlot
5. 36 yuva: her biri id (int32), count (int32), durability (int32)

Uçuş ve menü durumu yazılmaz. Açlık alanı eklenirse sürüm artar.

`SavePlayer` / `LoadPlayer` çıkış ve "dünyaya gir" anında. Otomatik kayıt: birkaç dakikada bir kirli chunk + oyuncu, kareyi bölmemek için aynı kuyruk.

Dünya adı v1 sabit: `Dunyam1`. Menüden ad, kayıt çalışınca.

## Ayar

```json
{
  "fov": 90,
  "renderDistance": 8,
  "mouseSensitivity": 0.1,
  "vsync": true,
  "bloom": false,
  "shadows": false
}
```

Yoksa bu varsayılanlarla oluştur. Menüden değişince hemen yaz ve canlı uygula:

- FOV → projeksiyon
- görüş mesafesi → yükleme yarıçapı (yeni chunk'lar bir sonraki `World::Update`)
- hassasiyet → fare çarpanı
- vsync → `SDL_GL_SetSwapInterval`
- bloom / shadows → geçiş atlanır, FBO kurulmaz

`GameSettings` tek nesne. Global serbest değişken çoğalmaz; `Game` sahibi olur, referans verir.

## Ana menü

ImGui. Oyun durumları `02`.

Pencere ortasında:

- Tek oyuncu → `Playing`, dünya yükle veya üret
- Çok oyunculu → IP kutusu + bağlan. Ağ mili yokken buton pasif, yanında "henüz yok" yazmasın; gizle (`11`)
- Ayarlar → aynı pencerenin ikinci sekmesi, yukarıdaki alanlar
- Çıkış

Oyun içi Esc: `Paused`, aynı ayar sekmesi + "Kaydet ve çık" + "Devam".

HUD ImGui değildir. ImGui dünya shader'ına karışmaz; kendi çizimini swap'tan hemen önce çağır.

Fare: menüde görünür ve serbest, oyunda gizli ve kilitli (SDL relative). Geçişte delta sıçramasını yut.
