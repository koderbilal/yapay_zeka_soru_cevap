# Canlılar ve savaş

## Taban

ECS yok. Ortak veri tabanda, davranış sanal `Update`.

```cpp
struct Entity {
    glm::vec3 position;
    glm::vec3 velocity;
    float yaw;
    float health;
    float hurtTime;
    glm::vec3 halfExtents;   // AABB yarı boyut
    bool alive;
    virtual void Update(float dt, World&, Player&) = 0;
    virtual void Render(const Camera&) = 0;
    virtual ~Entity() = default;
};
```

Liste `Game`de. Chunk düşünce canlı silinmez; oyuncudan çok uzaksa uyur (Update atlanır). Kayıt v1'de canlıyı diske yazmaz; dünya açılınca yeniden doğar. Bu boşluk bilinçli, boss'tan önce kapatılır.

Doğum: chunk ilk üretildiğinde (dosyadan yüklendiyse doğurma) yüzeyde seyrek zar. Ormanda tavuk, her biyomda gece zombi daha sık. Tavan sayısı: oyuncu çevresinde tavuk ≤ 8, zombi ≤ 6 gibi bir tavan, her kare sayım.

## Tavuk

Pasif. AABB küçük (~0.4 × 0.7 × 0.4).

- Rastgele aralıkla yaw değiştir, yavaş yürü, önü katıysa veya boşluksa dön.
- Oyuncu yaklaşırsa ters yöne kaç.
- Ölünce (`health <= 0`): `raw_chicken`, bazen `feather`, silin.
- Yumurta v1'de düşmez; eşya kayıtta durur, bırakma sonra.

## Zombi

AABB oyuncuya yakın (~0.6 × 1.9 × 0.6).

- Yatayda oyuncuya doğru yaw, yürüyüş hızı tavuktan yüksek.
- v1 yol bulma: düz yürüme + bir blok zıplama (önü katı, üstü hava). Merdiven, kapı, A* yok.
- Mesafe ~1.2 ve `hurtTime` bittiyse oyuncuya vur: `health` azalır, oyuncu `hurtTime` dolar (kısa iframes), hafif geri itme, vuruş sesi.
- Oyuncu ölünce v1: dünyada kal, can 20'ye çek, yatağın yerine başlangıç noktası. Ölüm ekranı sonra.
- Zombi ölünce `rotten_flesh`, parçacık, silin.

## Oyuncu vuruşu

Elinde kılıç veya yumruk. Sol tık, bakılan blok menzilde değilse (ışın bloğa çarpmadı) ya da saldırı bekleme süresi dolduysa:

- Kısa menzilli ikinci ışın veya öndeki AABB'lerle kesişim. İlk canlı.
- Hasar: yumruk az, tahta kılıç daha çok. Bekleme: kılıç ~0.6 sn.
- Hedef `hurtTime`, hafif knockback, kılıç durability −1, vuruş parçacığı.
- Blok kırma ile aynı tık: ışın önce bloğa bakılır. Menzilde katı blok varsa kırma, yoksa saldırı. İkisi bir karede olmaz.

Görsel: vurulan model kısa süre kırmızı çarpan (shader uniform), kamerada kısa sallama.

## Model

Tek küp değil, küp hiyerarşisi. Her parça yerel konum, boyut, eksen, açı.

Zombi (Minecraft oranına yakın, birim blok):

- Gövde, kafa, sol/sağ kol, sol/sağ bacak. Hepsi `oak` yerine zombi dokusu (Bedrock entity dokusu atlasa veya ayrı texture).
- Yürüme: `sin(zaman * hız) * genlik` bacaklara ters, kollara ters. Durunca açı 0'a yumuşar.
- Kafa oyuncuya bakar (zombi), sınırlı pitch.

Tavuk: gövde, kafa, gaga, sakal, iki bacak, iki kanat. Kanat kaçarken daha hızlı çırpar; v1'de uçmaz.

Çizim: parça parçası model matrisini çarp, küçük birim küpü (paylaşılan VAO) o matrisle çiz. Küp UV'si parça dikdörtgenine kesilir. Bu küp dünya chunk mesh'inden ayrı shader kullanabilir; aynı atlas şart değil, entity dokusu ayrı bağlanır.

Oyuncu kendi modelini v1'de görmez (birinci şahıs). Diğer oyuncular (`10-ag.md`) aynı zombi rig'i, Steve dokusu.
