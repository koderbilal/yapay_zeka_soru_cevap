# Oyuncu, fizik, eşya

## İki parça

`Camera` bakar. `Player` vardır.

```cpp
struct Player {
    glm::vec3 position;
    glm::vec3 velocity;
    float health;            // 0–20, yarım kalp = 1
    float hunger;            // alan durur, mekanik yok (01)
    bool onGround;
    bool flying;             // creative
    int activeSlot;          // 0–8
    InventorySlot slots[36]; // 0–8 hotbar, 9–35 çanta
    InventorySlot crafting[9];
    InventorySlot craftingResult;
    InventorySlot cursor;    // fareye yapışık
    bool craftingOpen;
    float breakProgress;     // 0–1, basılı tutma
    int breakX, breakY, breakZ;
    float hurtTime;          // vuruştan sonra kısa dokunulmazlık
};
```

Hitbox: genişlik ~0.6, yükseklik ~1.8, göz yüksekliği ~1.62. Kamera konumu = ayak + göz. Fizik ayak konumunu iter.

`InventorySlot`: `int id`, `int count` (en fazla 64), `int durability` (−1 ise yok). Aletlerde durability dolu başlar, kullanımda azalır, 0 olunca yuva boşalır.

## Fizik

Uçuş kapalıyken her kare:

- `velocity.y -= gravity * dt` (havada). Yerde sıfır.
- Zıplama yalnız `onGround` iken, hızın y'sine sabit.
- Hareketi **eksen eksen** uygula: önce X, çarpıştıysa X hızını kes; sonra Y; sonra Z. Böylece köşede takılmaz, duvarda kayar.
- Çarpışma: oyuncu AABB'sinin değdiği bloklar. `BlockDef.solid` olmayan (hava, su, sapling, torch, yaprak) içinden geçilir. Su hızı kısar.
- Creative: yerçekimi yok, WASD + Space/Ctrl bakış yönünde, çarpışma yok.

Bakış ayrı: fare `Yaw` ve `Pitch` (Pitch −89..89). Hassasiyet ayardan. `Front` sin/cos ile yeniden hesaplanır. Yürüme yatay `Front`'un y'si sıfırlanmış halidir; uçuşta ham `Front`.

## Işın

Gözden `Front` boyunca kısa adım (örneğin 0.05), menzil ~5 blok. İlk katı blok hedef. Bir önceki hava hücresi koyma yeri.

- Sol tık basılı: aynı bloğun `breakProgress`'i artar. Bırakınca veya hedef değişince sıfır.
- Süre `hardness / aletÇarpanı`. Doğru alet (kazma taşa, balta oduna, kürek toprağa) çarpanı büyütür. Yanlış alet veya yumruk yavaş. Bedrock ilerlemez.
- Dolunca: `drop = BlockDef.dropItem`, `World::SetBlock(air)`, `SpawnDrop(drop)`, kırılma parçacığı, kırılma sesi, alet durability −1.
- Sağ tık, elde yerleştirilebilir blok, koyma hücresi boş ve oyuncu AABB'sine girmiyorsa: `SetBlock`. Meşaleyse ışık yayılır.
- Artı imleç ekran ortası, HUD.

Seçili yuva 1–9 ve tekerlek. `activeSlot` hotbar çiziminde çerçeve.

## Toplama

Düşen eşya oyuncu kutusuna değince:

1. Aynı id, count < 64 olan yuva.
2. Yoksa ilk boş yuva.
3. İkisi de yoksa yerde kalır.

Hotbar önce aranır, sonra çanta. 9. turun "kırınca doğrudan yuva" kodu yazılmaz.

## Üretim

Çalışma masasına sağ tık: `craftingOpen`, fare serbest, dünya durmaz (v1'de duraklatmak da olur; seçilen: Minecraft gibi açık dünya, imleç serbest).

`Recipe`: `int in[9]`, `int resultId`, `int resultCount`. Başlangıç tarifleri:

- Tahta: 1 odun → 4 `oak_planks` (2×2 köşede de tutsun diye tarif hem 3×3 sabit konum hem "şekil kaydırmalı" aranır; v1 kaydırmalı tek fonksiyon, shapeless değil).
- Çubuk: dikey 2 tahta → 4 `stick`.
- Ahşap kazma: üst sıra 3 tahta, orta ve alt orta çubuk.
- Ahşap kılıç: orta sütun tahta, tahta, çubuk.
- Çalışma masası: 2×2 tahta → 1 masa.

Izgara her değişince tarif listesi taranır, uyan `craftingResult`'a yazılır. Sonuç yuvasına tıklanınca malzeme 1 azalır (sonuç adedi tarifte), sonuç imlece veya envantere. Uymayan sonuç temizlenir.

2×2 oyuncu üretimi (çanta açıkken) aynı fonksiyon, kullanılmayan 5 hücre hava.

Sürükle-bırak: sol tık yuvayı imlece alır veya koyar; sağ tık yarısını ayırır. v1 sol tık yeter, sağ tık sonra.

## HUD'un okuduğu

Kalp: `health / 2` ikon, Bedrock kalp dokusundan UV kırpma (dolu, yarım, boş). Hotbar dokusu alt orta. Slot içeriği atlas ikonu. Sayı sağ alt, `ascii.png` glifleri (`06`). Seçili slot çerçevesi bir piksel büyük çizilir. Kırma çatlağı, bakılan bloğun üstüne 0–9 kademe doku.

Açlık ikonları bu dosyada yer tutmaz; alan `Player.hunger` olarak durur, çizim açlık tasarlanınca eklenir.
