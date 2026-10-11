# Dünya

## Kimlikler

`Registry` oyun açılışında dolar. İleride kaynak JSON olur; şimdilik C++ tablosu da olur, şekil aynı kalsın.

```cpp
struct BlockDef {
    uint8_t id;
    const char* name;          // "minecraft:dirt"
    bool solid;                // çarpışma
    bool opaque;               // arkadaki yüzü kapatır mı, ışık keser mi
    bool transparentPass;      // su, yaprak, cam: ikinci mesh
    uint8_t lightEmission;     // meşale 14, çoğu 0. Aralık 0–15
    float hardness;            // kırma süresi
    uint8_t preferredTool;     // 0 yok, 1 kazma, 2 balta, 3 kürek
    int dropItem;              // kırılınca düşen. Taş -> cobblestone gibi farklı olabilir
    int atlas[6];              // yüz başına atlas hücresi: +Y -Y +X -X +Z -Z
};
```

Yerleştirilebilir bloklar 1–255. 0 havadır: çizilmez, katı değildir, ışık taşır.

İlk liste (17. tur) ve sonradan eklenenler aynı tabloda durur:

| id | ad | not |
|---|---|---|
| 0 | air | |
| 1 | stone | düşünce cobblestone (kazmasız yine cobble, v1) |
| 2 | granite | |
| 3 | diorite | |
| 4 | andesite | |
| 5 | grass_block | üst yüz ayrı UV |
| 6 | dirt | |
| 7 | cobblestone | |
| 8 | oak_log | üst/alt kabuksuz |
| 9 | oak_planks | |
| 10 | oak_sapling | çapraz veya ince; katı değil |
| 11 | bedrock | kırılmaz, `hardness` sonsuz |
| 12 | sand | |
| 13 | oak_leaves | saydam geçiş, `discard` |
| 14 | cactus | |
| 15 | dead_bush | çöl süs. Bedrock adı bu; "dead shrub" ayrı blok değil |
| 16 | short_grass | süs, katı değil. İlk mesajdaki "Grass" bitkisi |
| 17 | fern | süs, katı değil |
| 18 | water | saydam, katı değil, oyuncu yavaşlar |
| 19 | crafting_table | sağ tık menü açar |
| 20 | torch | ışık 14, ince model sonra; v1 dik kutucuk yeter |
| 21–40 | iron/diamond cevher, kömür, cam | mağara turu bunları ister, id'yi bu aralıkta sabitle |

Eşyalar (chunk'a girmez, `int` kimlik ≥ 256):

| id | ad |
|---|---|
| 256 | apple |
| 257 | raw_chicken |
| 258 | cooked_chicken |
| 259 | feather |
| 260 | egg |
| 261 | rotten_flesh |
| 262 | stick |
| 263 | wooden_pickaxe |
| 264 | wooden_sword |

Elma, tüy, yumurta, çiğ/pişmiş tavuk, çürük et dünyaya blok olarak konmaz. Sağ tıkta `id < 256` değilse `SetBlock` çağrılmaz. Ham sohbetteki 50, 260, 261 numaraları eskidir; geçerli olan bu tablodur.

## Chunk

16 × 256 × 16. İki dizi, ikisi de `uint8_t`, 64 KB + 64 KB:

- `blocks[x][y][z]`
- `lightMap[x][y][z]` — 0..15. Katı blokta her zaman 0. Işık hava, yaprak, suda durur.

Koordinat: `chunkX`, `chunkZ`. Dünya konumu `(chunkX * 16 + x, y, chunkZ * 16 + z)`.

Sınır kutusu, frustum için:

- min `(chunkX * 16, 0, chunkZ * 16)`
- max `((chunkX + 1) * 16, 256, (chunkZ + 1) * 16)`

Mesh:

- `opaqueVertices` + VAO/VBO
- `transparentVertices` + VAO/VBO
- `state`: `Empty`, `Generating`, `ReadyToUpload`, `Uploaded`
- `dirty` — blok değişince true, worker yeniden meshler

Komşu yüz chunk sınırını aşar. Üstteki blok yan chunk'taysa `World::GetBlock` bakar, dizi dışına `blocks[x+1]` yazılmaz. Yoksa chunk kenarında delik kalır.

## Yüz eleme

Hava veya saydam komşusu olan yüz mesh'e girer. Tamamı dolu chunk'ta iç yüz hiç gitmez. Bir chunk = bir `glDrawArrays` (opak) + gerekirse bir tane daha (saydam). Blok başına draw yok.

Köşe verisi, yüz başına: konum (3) + uv (2) + ışık (1) + AO (1). Çizimde `vertexCount = bytes / 7`, erken cevaplardaki `/5` eskidi; ışık ve AO eklenince adım 7.

UV, atlas hücresinden. 16×16 doku, atlas N hücreyse hücre boyu `1/N`. `GL_NEAREST` hem küçültürken hem büyütürken. Mipmap istiyorsan `GL_NEAREST_MIPMAP_LINEAR`; lineer filtre yok.

Atlas kendin paketlenir: Bedrock `textures/blocks/*.png` aynı ızgaraya. İsim eşlemesi `BlockDef.atlas[]`.

## Üretim

Sıra, bir chunk ilk kez doğarken:

1. Diskte `{x}_{z}.chunk` var mı? Varsa diziyi oku, üretme (`08`).
2. Yoksa:
   - Yükseklik gürültüsü, frekans ~0.01. Taban 64, genlik ~30.
   - Sıcaklık gürültüsü, daha yavaş (~0.005). `> 0.3` çöl, değilse orman. İleride üçüncü bant (dağ) eklenebilir; v1 iki biyom.
   - Çöl: kum yüzey, kaktüs, dead bush. Orman: çimen, toprak, yer yer ağaç, eğrelti.
   - Ağaç: yükseklik yüzeyinde, seyrek eşik. Gövde `oak_log` 4–6, tepe `oak_leaves`. Yaprak komşu chunk'a taşıyorsa o chunk da kirlenir.
   - Su: belirli yüksekliğin (örneğin 62) altındaki hava su olur. Nehir, vadinin bu seviyenin altına düşmesiyle kendiliğinden çıkar. Akışkan simülasyonu yok; kaynak blok durur.
   - Yeraltı: 3D gürültü (FastNoiseLite) ile mağara. Taş katmanı yüzeyin altı. Damar: ikinci 3D gürültü eşiği, demir daha sığ, elmas daha derin.
   - `y == 0` bedrock.
3. Gök ışığı: sütun sütun en üst havadan aşağı, opak blok kesene kadar 15.
4. Meşale ve gök için flood fill (`PropagateLight`).
5. Mesh.

Tohum `World`da sabit `int seed`. Bütün gürültüler buna bağlı. Kayıtlı chunk üretilmez, tohum sadece yeni arazi içindir.

## Işık

`lightMap` 0–15.

- Gök ışığı sütunu, gece çarpanı shader'da. Haritayı her kare yeniden doldurma.
- Meşale: `SetBlock` sonrası BFS. Kuyruk `World`da, çünkü ışık chunk sınırını geçer.
- Komşu, kaynaktan 1 az alır. Opak blok yaymaz.
- Meşale kırma: sadece 0 yazmak yetmez, eski ışık oyuk bırakır. Söndürme BFS'i de `World`da. Bu sohbette yarım kaldı; `SetBlock(torch -> air)` onsuz bitmiş sayılmaz.
- Mesh köşesine o hücrenin ışığı yazılır. Shader: `renk * (ışık / 15) * gündüzÇarpanı`.

Yüzey gölgelemesi ışıktan ayrı, ucuz ve her zaman açık:

- Üst yüz 1.0, yan 0.8, alt 0.5. Köşeye düz renk çarpanı.
- AO: yüzün iki yan komşusu ve köşe komşusu doluysa o köşe kararır. Dört köşe, 0–3. Shader çarpar.

## Su ve saydam

- Opak geçiş önce, derinlik yazarak.
- Saydam geçiş derinlik yazar ama testi açık kalır; ya kameraya göre sırala ya da en azından chunk'ları uzaktan yakına çiz.
- Yaprak ve cam: alfa eşiğinin altı `discard`. Böylece arkası görünür, sıralama hatası cam kadar bozuk olmaz.
- Su: yarı saydam mavi, ikinci mesh. İçine girince post bulanıklığı `06`.

## Dünya akışı

`World` yüklenen chunk'ları `map<(cx,cz), Chunk>` tutar. Her kare oyuncu chunk'ına bak:

- Görüş mesafesi (ayar, varsayılan 8 chunk) içindeki eksikler üretim kuyruğuna.
- Dışarı çıkanlar: kirliyse kaydet, sonra RAM'den sil.
- Aynı anda üretilen işçi sayısı sınırlı (2–4). Her chunk için `detach` edilmiş başıboş thread yok; havuz var. 14. turun `detach` örneği basitti, sınıf olarak havuz esas alınır.

Ayrıntılı durum makinesi `07-performans.md`.
