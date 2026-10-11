# Ekran, ses, atmosfer

## Shader'lar

Hepsi `assets/shaders/`. Yükleyen `ReadFile` (`02`).

| Çift | İş |
|---|---|
| `chunk.vert/frag` | dünya. UV, yüz çarpanı, AO, ışık, gündüz |
| `entity.vert/frag` | küp parçaları, hurt renk çarpanı |
| `ui.vert/frag` | 1×1 quad, piksel ortho, UV kırpma (ikon ve glif) |
| `sky.vert/frag` | güneş ve ay billboard, gökyüzü rengi ayrı, `glClearColor` |
| `particle.vert/frag` | instanced, kameraya bakan quad |
| `post.vert/frag` | FBO'dan ekrana. Bloom, su altı, renk. Yokken dünya doğrudan ekrana |

Chunk fragment kabaca: doku × yüzÇarpanı × ao × max(blokIşığı, gökIşığı × gündüz). Gece gök ışığı 0'a yaklaşır, meşale kalır.

Derinlik testi dünyada açık. UI'da kapalı. Saydamda blend açık: `SRC_ALPHA, ONE_MINUS_SRC_ALPHA`.

## HUD

Tek birim quad. Model matris ekran pikselinde konum ve boyut verir. Ortho: sol üst (0,0), sağ alt (genişlik, yükseklik), Y aşağı.

Çizim sırası, 3D bittikten sonra:

1. Hotbar dokusu, alt orta.
2. Dokuz ikon, slot doluysa atlas UV kırpma.
3. Adet > 1 ise sağ alt glif (`ascii.png`, Bedrock font). Her karakter bir hücre.
4. Seçili yuva çerçevesi.
5. Kalpler sol üst. Dolu / yarım / boş UV.
6. Artı, tam orta.
7. Kırma çatlağı dünya uzayında, HUD shader'ında değil; chunk shader'ında hafif koyu örtü veya ayrı quad bloğun yüzünde.

Pencere boyutu değişince ortho ve FBO yeniden kurulur.

## Gün ve gece

`timeOfDay` 0–1, bir tam gün sabit gerçek saniye (örneğin 20 dakika). Hız ayarı yok, v1.

- 0.25 öğle, 0.75 gece yarısı. Eşikler yumuşak karışım.
- Temizleme rengi iki `vec3` arası (gündüz mavi, gece lacivert).
- Gündüz çarpanı 1 → ~0.15. Zifiri siyah yok; meşale olmasa da siluet kalsın.
- Güneş ve ay: kameraya uzak, `timeOfDay` açısıyla dönen iki quad. Biri batınca diğeri çıkar.

Bu, flood fill'den bağımsızdır. İkisi shader'da çarpılır.

## Parçacık

Havuz. Sabit dizi (örneğin 4096). Ölü yuva `life <= 0`. Her kare yaş azalt, yerçekimi, `glDrawArraysInstanced`. Quad vertex shader'da kameranın sağı ve yukarısıyla billboard olur.

Tetik:

- Blok kırılınca 8–12 tane, o bloğun atlas renginden, kısa ömür.
- Vuruşta az, kırmızımsı.
- Koşan oyuncuda seyrek toz. v1 isteğe bağlı.

Yeni `new` yok. Havuz doluysa en eskiyi ez.

## Hava

Aynı havuzdan ayrı liste de olabilir; yağmur çok parçacık ister, kendi sabit havuzu daha temiz (birkaç bin).

- Yağmur: oyuncunun çevresinde, yüksekte rastgele xz, aşağı hız. Yüksekliği o sütunun yüzeyine düşünce ölür veya sıçrama parçacığı. Biyom çölse o sütunda üretme.
- Kar: sıcaklık eşiği soğuk bant. v1'de sıcaklık < −0.2 ise kar, yoksa yağmur, çölde hiçbiri. Daha yavaş, daha büyük quad.
- Hava durumu dünya zamanına bağlı seyrek zar: açık / yağmur. Fırtına şiddeti sonra.

Yükseklik testi için sütunun en üst katı bloğu lazım. Chunk yüklüyse oradan, değilse üretme.

## Ses

`AudioEngine` miniaudio. Kısa sesler bellekten, 3D.

| Olay | Ses | Konum |
|---|---|---|
| Yürüme, `onGround`, belirli mesafe | yüzey tipine göre adım (toprak, taş, çimen) | oyuncu |
| Kırma tamam | blok ailesi | blok |
| Kılıç / yumruk | vuruş | hedef |
| Zombi, aralıklı | hırıltı | zombi, mesafe kısar |
| Menü tık | UI | 2D, konum yok |

Dinleyici her kare kamera konumu ve `Front`. Aynı anda aynı adım sesi üst üste binmesin, sayaç.

Dosya adları Bedrock `sounds/` düzenine yakın tutulsun ki paket değişince yol şeması bozulmasın.

## Post ve gölge

İkisi de FPS yer. Yol haritasında dünya oynanır olduktan sonra. Kapalıyken kod yolları derlenir ama `settings` ile kapanabilir olmalı.

Post:

- Dünya bir FBO rengine çizilir.
- İkinci geçiş ekran quad'ı: hafif renk düzeltme her zaman ucuz.
- Bloom: parlak pikselleri eşiğe ayır, bulanıklaştır, üstüne ekle. Güneş ve meşale işe yarar.
- Su altındayken (göz hücresi su): mavi çarp + bulanık. Değilse atla.

Gölge:

- Güneş yönünden ortografik harita, ayrı derinlik FBO.
- Yalnız opak chunk ve varlıklar bu haritaya çizilir. Su ve parçacık gölge düşürmez (v1).
- Asıl shader'da parça gölgede mi diye karşılaştır. Akne için bias.
- Güneş batınca haritayı güncelleme, ucuz ambient yeter.
- Frustum (dünya çizimi) ile gölge kamerası ayrı. Gölge kamerası oyuncunun çevresini görür, arkasını da. "Arkayı hiç çizme" gölgeyi bozar; o red yalnız ana kamera içindir (`07`).
